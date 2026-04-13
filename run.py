import subprocess
import os
import numpy as np
import re
import argparse
import sys

BINARY_MAP = {
    "r0": "./build/run-r0",
    "r1": "./build/run-r1",
    "access": "./build/run-access",
    "ut": "./build/unit-test"
}

def build_project():

    print("Building binaries...")
    
    try:
    
        subprocess.run(["make"], check=True, capture_output=True)
    
    except subprocess.CalledProcessError:
    
        print("Error: 'make' command failed.")
    
        sys.exit(1)

def run_benchmark(binary_path, max_bitvector_size, no_of_operations):

    final_summary = []
    graph_size = max_bitvector_size
    input_size = 1
    
    while input_size < 10**graph_size:
        input_size *= 10
        curr_runs = []
        print(f"\n[Benchmarking] Size: {input_size}")
        
        while True:
            result = subprocess.run([binary_path, str(input_size), no_of_operations], capture_output=True, text=True)
            
            data_points = tuple(map(float, re.findall(r"\d+\.?\d*", result.stdout.strip())))
            if not data_points:
                continue
                
            curr_runs.append(data_points)
            
            if len(curr_runs) % 2 == 0:
                print(f"\r  > Completed {len(curr_runs)} runs...", end="")

            if len(curr_runs) < 10:
                continue
            
            run_times = [r[1] for r in curr_runs]
            median = np.median(run_times)
            p25 = np.percentile(run_times, 25)
            p75 = np.percentile(run_times, 75)
            
            ratio = (p75 - p25) / median if median != 0 else 0
            
            if ratio <= 0.05 or len(curr_runs) >= 50:
                print(f"\n  > Finished with {len(curr_runs)} runs (Ratio: {ratio:.4f})")
                break
        
        final_summary.append((input_size, median, len(curr_runs), p25, p75))

    print(f"{'Input Size':<12} | {'Q2 (ns)':<12} | {'Runs':<5} | {'Q1 (ns)':<12} | {'Q3 (ns)':<12} | {'is_unstable?'}")
    
    for size, med, count, p25, p75 in final_summary:
        is_unstable = False
        if count >= 50:
            is_unstable = True
        print(f"{size:<12} | {med:<12.6f} | {count:<5} | {p25:<12.6f} | {p75:<12.6f} | {is_unstable}")


def main():

    parser = argparse.ArgumentParser(description="Benchmarking & Unit Testing CLI")
    
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-r0", action="store_true", help="Benchmark R0 binary")
    group.add_argument("-r1", action="store_true", help="Benchmark R1 binary")
    group.add_argument("-access", action="store_true", help="Benchmark Access binary")
    group.add_argument("-ut", action="store_true", help="Run Unit Tests binary")

    parser.add_argument("-s", "--size", type=int, default=8, help="Max bitvector size (power of 10), default 8")
    parser.add_argument("-o", "--ops", type=str, default="1e7", help="Number of operations, default 1e7")
    
    args = parser.parse_args()

    if args.r0: selected = "r0"
    elif args.r1: selected = "r1"
    elif args.access: selected = "access"
    else: selected = "ut"

    target_binary = BINARY_MAP[selected]

    build_project()

    if selected == "ut":

        subprocess.run([target_binary], check=True)

    else:
        
        run_benchmark(target_binary, args.size, args.ops)

if __name__ == "__main__":
    main()