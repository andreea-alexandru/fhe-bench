## Running the "add two numbers" workload

```console
git clone https://github.com/andreea-alexandru/fhe-bench
cd fhe-bench
python3 harness/run_submission.py -h # Provide information about command-line options
```

The first time you run `harness/run_submission.py`, it will attempt to pull and build OpenFHE if it is not already installed, and will then build the submission itself. 
On subsequent calls it will use the same project without re-building it unless the code has changed. An example run is provided below.

```console
$ python3 harness/run_submission.py -h
usage: run_submission.py [-h] [--num_runs NUM_RUNS] [--seed SEED] [--clrtxt CLRTXT] {0,1,2,3}

Run the add-two-values FHE benchmark.

positional arguments:
  {0,1,2,3}            Instance size (0-toy/1-small/2-medium/3-large)

options:
  -h, --help           show this help message and exit
  --num_runs NUM_RUNS  Number of times to run steps 4-9 (default: 1)
  --seed SEED          Random seed for dataset and query generation
  --clrtxt CLRTXT      Specify with 1 if to rerun the cleartext computation

$ python3 ./harness/run_submission.py 2 --seed 3 --num_runs 2

[harness] Running submission for medium dataset
[get-openfhe] Found OpenFHE installed at /usr/local/lib/ (use --force to rebuild).
-- FOUND PACKAGE OpenFHE
-- OpenFHE Version: 1.3.0
-- OpenFHE installed as shared libraries: ON
-- OpenFHE include files location: /usr/local/include/openfhe
-- OpenFHE lib files location: /usr/local/lib
-- OpenFHE Native Backend size: 64
-- Configuring done
-- Generating done
-- Build files have been written to: /home/aandr/fhe-bench/submission/build
Consolidate compiler generated dependencies of target server_preprocess_dataset
Consolidate compiler generated dependencies of target client_postprocess
Consolidate compiler generated dependencies of target client_encode_encrypt_db
Consolidate compiler generated dependencies of target client_key_generation
Consolidate compiler generated dependencies of target server_encrypted_compute
Consolidate compiler generated dependencies of target client_preprocess_dataset
Consolidate compiler generated dependencies of target client_preprocess_query
Consolidate compiler generated dependencies of target client_decrypt_decode
Consolidate compiler generated dependencies of target client_encode_encrypt_query
[ 11%] Built target server_preprocess_dataset
[ 33%] Built target client_postprocess
[ 33%] Built target server_encrypted_compute
[ 44%] Built target client_encode_encrypt_db
[ 55%] Built target client_key_generation
[ 66%] Built target client_preprocess_query
[ 77%] Built target client_preprocess_dataset
[ 88%] Built target client_decrypt_decode
[100%] Built target client_encode_encrypt_query
18:30:26 [harness] 1: Dataset generation completed (elapsed: 0.2908s)
18:30:26 [harness] 2: Dataset preprocessing completed (elapsed: 0.0158s)
18:30:26 [harness] 3: Key Generation completed (elapsed: 0.0699s)
18:30:26 [harness] 4: Dataset encoding and encryption completed (elapsed: 0.0422s)
         [harness] Public and evaluation keys size: 517.8K
         [harness] Encrypted database size: 261.2K
18:30:26 [harness] 5: (Encrypted) dataset preprocessing completed (elapsed: 0.1851s)

         [harness] Run 1 of 2
18:30:26 [harness] 6: Query generation completed (elapsed: 0.2721s)
18:30:26 [harness] 7: Query preprocessing completed (elapsed: 0.0667s)
18:30:26 [harness] 8: Query encryption completed (elapsed: 0.0931s)
         [harness] Encrypted query size: 257.2K
18:30:27 [harness] 9: Encrypted computation completed (elapsed: 0.1602s)
         [harness] Encrypted results size: 261.2K
18:30:27 [harness] 10: Result decryption completed (elapsed: 0.1451s)
18:30:27 [harness] 11: Result postprocessing completed (elapsed: 0.0732s)
         [harness] Wrote expected result to:  /home/aandr/fhe-bench/datasets/medium/expected.txt
[harness] PASS  (expected=13.89, got=13.890000000104857)
[total latency] 1.4143s

         [harness] Run 2 of 2
18:30:27 [harness] 6: Query generation completed (elapsed: 0.4365s)
18:30:27 [harness] 7: Query preprocessing completed (elapsed: 0.0711s)
18:30:27 [harness] 8: Query encryption completed (elapsed: 0.0749s)
         [harness] Encrypted query size: 257.2K
18:30:28 [harness] 9: Encrypted computation completed (elapsed: 0.1586s)
         [harness] Encrypted results size: 261.2K
18:30:28 [harness] 10: Result decryption completed (elapsed: 0.1466s)
18:30:28 [harness] 11: Result postprocessing completed (elapsed: 0.0924s)
         [harness] Wrote expected result to:  /home/aandr/fhe-bench/datasets/medium/expected.txt
[harness] PASS  (expected=123.58, got=123.58000000083385)
[total latency] 1.5841s

All steps completed for the medium dataset!
```

Directory structure: each submission to the workload in the FHE benchmarking 
is a branch of the repository, with (a subset of) the following directory structure:
[root] /
 ├─datasets/   # Holds cleartext data 
   ├─ toy/     # each instance-size in in a separate subdirectory
   ├─ small/
   ├─ medium/
   ├─ large/
 ├─docs/       # Documentation (beyond the top-level README.md)
 ├─harness/    # Scripts to generate data, run workload, check results
 ├─build/      # Handle installing dependencies and building the project
 ├─submission/ # The implementation, this is what the submitters modify
   └─ README.md  # likely also a src/ subdirectory, CMakeLists.txt, etc.
 ├─io/         # Directory to hold the I/O between client & server parts
   ├─ toy/       # The reference implementation has subdirectories
      ├─ public_keys/       # holds the public evaluation keys
      ├─ ciphertexts_download/  # holds the ciphertexts to be downloaded by the client
      ├─ ciphertexts_upload/  # holds the ciphertexts to be uploaded by the client    
      ├─ intermediate/  # internal information to be passed around the functions
      └─ secret_key/  # holds the secret key
   ├─ small/
      …
   ├─ medium/
      …
   ├─ large/
      …
 ├─measurements/   # Holds json files with the results for each run
   ├─ toy/     # each instance-size in in a separate subdirectory
   ├─ small/
   ├─ medium/
   ├─ large/



A submitter can edit any of the `client_*` / `server_*` sources in `/submission`. 
Moreover, for the particular parameters related to a workload, the submitter can modify the params files.
If the current description of the files are inaccurate, the stage names in `run_submission` can be also 
modified.

The current stages are the following, targeted to a client-server scenario.
The order in which they are happening in `run_submission` assumes an initialization step which is 
database-dependent and run only once, and potentially multiple runs for multiple queries.
Each file can take as argument the test case size.


| Stage executables         
|---------------------------------
| client_key_generation: Generate all key material and cryptographic context at the client.           
| client_preprocess_dataset: (Optional) Any in the clear computations the client wants to apply over the dataset/model.
| client_preprocess_query: (Optional) Any in the clear computations the client wants to apply over the query/input.
| client_encode_encrypt_db: (Optional) Plaintext encoding and encryption of the dataset/model at the client.
| client_encode_encrypt_query: Plaintext encoding and encryption of the query/input at the client.
| server_preprocess_dataset: (Optional) Any in the clear or encrypted computations the server wants to apply over the dataset/model.
| server_encrypted_compute: The computation the server applies to achieve the workload solution over encrypted daa.
| client_decrypt_decode: Decryption and plaintext decoding of the result at the client.
| client_postprocess: (Optional) Any in the clear computation that the client wants to apply on the decrypted result.


The outer python script measures the runtime of each stage.
The current stage separation structure requires reading and writing to files more times than minimally necessary.
For a more granular runtime measuring, which would account for the extra overhead described above, we encourage
submitters to separate and print in a log the individual times for reads/writes and computations inside each stage. 