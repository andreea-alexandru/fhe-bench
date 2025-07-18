#!/usr/bin/env python3
"""
If the datasets is are too large to include, generate them here or pull them 
from a storage source.
"""
import random
from pathlib import Path

test_cases = ["small", "medium", "large"]
bounds = [3, 300, 3000]
BASELINE_DIR = Path(__file__).resolve().parent   
DATASET_DIR = BASELINE_DIR.parent / "datasets"      

for cnt,test in enumerate(test_cases):
    DATASET_DB_PATH = DATASET_DIR / f"DB_{test}.txt"
    db = round(random.uniform(-bounds[cnt], bounds[cnt]), 2)
    DATASET_DB_PATH.write_text(f"{db}\n", encoding="utf-8")

