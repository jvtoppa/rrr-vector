import subprocess
import os
import numpy as np
import re
binary_path = "./build/unit-test"

if not os.path.exists(binary_path):
    print(f"'{binary_path}' not found. Attempting to build...")
    
    try:
        subprocess.run(["make"], check=True)
        print("Build successful.")
    except:
        print("Error: 'make' failed.")
        exit(1)
else:
     subprocess.run(["make","clean"], check=True)
     subprocess.run(["make"], check=True)
     print("Rebuilt the binary.")


runs = []
graph_size = 8
input_size = 1
position = 0
while input_size < 10**graph_size:
    
    input_size *= 10
    position += 1
    curr_runs = []
    print("Running experiment for input size =", input_size)
    while True:
        result = subprocess.run([binary_path, str(input_size), "1e7"], capture_output=True, text=True)
        result = tuple(map(float, re.findall(r"\d+\.?\d*", result.stdout.strip())))
        curr_runs.append(result)
        if len(curr_runs) % 5 == 0:
            print("Done", len(curr_runs), "runs")
        
        if len(curr_runs) < 10:
            continue
        
        run_times = [r[1] for r in curr_runs]
        median = np.median(run_times)
        p25 = np.percentile(run_times, 25)
        p75 = np.percentile(run_times, 75)
        ratio = (p75 - p25) / median
        print("Ratio:", ratio)
        if ratio > 0.05 and len(curr_runs) < 50:
            continue
        print("Finished with",len(curr_runs), "runs")
        break
    
    runs.append(curr_runs)


print(runs)
