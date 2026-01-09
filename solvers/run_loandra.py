import os

TL = "10000s"

os.makedirs("loandra_results", exist_ok=True)
os.makedirs("loandra_logs", exist_ok=True)
os.makedirs("loandra_times", exist_ok=True)

with open("params.txt") as f:
    for l in f.readlines():
        if len(l.strip()) == 0:
            continue
        for test_number in range(10):
            arguments = (str(test_number) + " " + l.strip())
            out_name = '_'.join(arguments.split())+".txt"
            os.system(f"time timeout {TL} python3 call_loandra_binary.py {arguments} > loandra_times/{out_name} 2>&1")