#!/usr/bin/env python3
import subprocess, pathlib, json, shutil, time, sys, os

TASK_DIR = pathlib.Path(__file__).parents[0]          # .../harness
ROOT     = TASK_DIR.parents[0]                        # repo root
MEASURE_BIN = ROOT/"measure_io/io_size"
MEASURE_SRC = ROOT/"measure_io/io_size.cpp"          

# ---------- build io_size if missing (delete the executable to recompile) -------------------------------
if not MEASURE_BIN.exists():
    print("[run]  compiling measure_io/io_size …")
    MEASURE_BIN.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(["g++", "-std=c++17", "-O2", "-s",
                    "-o", str(MEASURE_BIN), str(MEASURE_SRC)],
                   check=True)

sub_dir  = pathlib.Path(sys.argv[1]).resolve()
build_dir= sub_dir/"build"

# 0. Generate datasets and queries for all test cases and the expected results
# Comment out the dataset generation after generating it once if desired.
dataset_py = ROOT / "baseline" / "generate_dataset.py"
subprocess.run(["python3", dataset_py], check=True)
baseline_py = ROOT / "baseline" / "cleartext_impl.py"
subprocess.run(["python3", baseline_py], check=True)

# 1. Build submission
subprocess.run([ROOT/"scripts/build_task.sh", str(sub_dir)], check=True)

# For each test case, run the submission overwriting the intermediate files
test_cases = ["small", "medium", "large"]
for test in test_cases:
    print("\nTest case: ", test)

    # 2. Wipe and recreate IO dir
    io_dir   = TASK_DIR.parent/"io"

    shutil.rmtree(io_dir, ignore_errors=True)
    io_dir.mkdir()
    (io_dir/"public_keys").mkdir()
    (io_dir/"secret_key").mkdir()
    (io_dir/"ciphertexts_upload").mkdir()
    (io_dir/"ciphertexts_download").mkdir()
    (io_dir/"intermediate").mkdir()

    # 3. Run submission and measure latency
    bin_dir = sub_dir / "build"
    stage_args = {
        "client_key_generation": [],
        "client_preprocess": [test],
        "client_encode_encrypt_db": [],
        "client_encode_encrypt_query": [],
        "server_preprocess": [],
        "server_encrypted_compute": [],
        "client_decode_decrypt": [test],
        "client_postprocess": [],
    }

    timings = {}
    for name, args in stage_args.items():
        exe = bin_dir / name
        if not exe.exists():
            raise FileNotFoundError(f"Missing stage binary: {exe}")
        
        t0 = time.perf_counter()
        subprocess.run([exe, *args], check=True, cwd=sub_dir)  # add test case arg if needed
        t1 = time.perf_counter()

        timings[name] = round((t1 - t0) * 1e3, 4)  # ms, 4 decimal digits

    # 4. Correctness check via verify_result.py
    exp = TASK_DIR.parent / "datasets" / f"expected_{test}.txt"
    got = TASK_DIR.parent / "io" / f"result_{test}.txt"

    res = subprocess.run(["python3", TASK_DIR/"verify_result.py", exp, got])
    if res.returncode != 0:
        raise RuntimeError("Verification failed; see message above")

    # 5. Measure bandwidth
    bytes = {}
    bytes["public_keys folder"] = int(subprocess.check_output([MEASURE_BIN, "io/public_keys"],cwd=TASK_DIR.parent).decode())
    bytes["ciphertexts_upload folder"] = int(subprocess.check_output([MEASURE_BIN, "io/ciphertexts_upload"],cwd=TASK_DIR.parent).decode())
    bytes["ciphertexts_download folder"] = int(subprocess.check_output([MEASURE_BIN, "io/ciphertexts_download"],cwd=TASK_DIR.parent).decode())
    
    # 6. Save and print results
    json.dump({
        "total_latency_ms": round(sum(timings.values()), 4),
        "per_stage_ms": timings,
        "bandwidth_bytes": bytes,
    }, open(sub_dir/f"results_{test}.json","w"), indent=2)

    print("[total latency]", round(sum(timings.values()), 4), "ms\n"
          "[latency per stage]", timings, "ms\n"
          "[bandwidth per round]", bytes, "bytes")
