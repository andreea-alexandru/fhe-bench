#!/usr/bin/env python3
"""
Cleartext reference for the “add” workload.
For each test case
• Reads  datasets    
• Computes a + b 
• Writes the result to expected_XXX.txt for each test case (# baseline/expected.txt)

If the queries should be generated at random for each test, write the code to generate them here
"""
import random
from pathlib import Path

test_cases = ["small", "medium", "large"]
BASELINE_DIR = Path(__file__).resolve().parent   
DATASET_DIR = BASELINE_DIR.parent / "datasets"      

for test in test_cases:
    DATASET_DB_PATH = DATASET_DIR / f"DB_{test}.txt"
    DATASET_Q_PATH = DATASET_DIR / f"query_{test}.txt"

    # 0) generate the query if desired
    q = round(random.uniform(-1000,1000), 2)
    DATASET_Q_PATH.write_text(f"{q}\n", encoding="utf-8")

    # 1) load database and the query if not already stored
    db = float(DATASET_DB_PATH.read_text().strip())
    # q = float(DATASET_Q_PATH.read_text().strip())

    # 2) compute reference result
    result = db + q

    # 3) write to expected.txt (overwrites if it already exists)
    OUT_PATH = DATASET_DIR / f"expected_{test}.txt"
    OUT_PATH.write_text(f"{result}\n", encoding="utf-8")
    print(f"[baseline] Wrote expected result to {OUT_PATH.relative_to(Path.cwd())}")

