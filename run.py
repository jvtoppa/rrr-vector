import subprocess
import os

binary_path = "./build/unit-test"

if not os.path.exists(binary_path):
    print(f"'{binary_path}' not found. Attempting to build...")
    
    try:
        subprocess.run(["make"], check=True)
        print("Build successful.")
    except:
        print("Error: 'make' failed.")
        exit(1)


result = subprocess.run([binary_path], capture_output=True, text=True)

# Access the results
print("Output:", result.stdout.strip())
with open("results.txt", "w") as f:
        f.write(result.stdout)
print("Exit Code:", result.returncode)