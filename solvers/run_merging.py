import os
import sys

TL = "1000s"
block_size = int(sys.argv[1])
restarts = int(sys.argv[2])

times_folder = f"merging_times_block{block_size}_restart{restarts}"
logs_folder = f"merging_logs_block{block_size}_restart{restarts}"

os.makedirs(times_folder, exist_ok=True)
os.makedirs(logs_folder, exist_ok=True)

with open("params.txt") as f:
    for l in f.readlines():
        if len(l.strip()) == 0:
            continue
        for test_number in range(10):
            arguments = (str(test_number) + " " + l.strip() + " " + str(block_size) + " " + str(restarts))
            out_name = '_'.join(arguments.split())+".txt"
            os.system(f"time timeout {TL} ./merging/main {arguments} {logs_folder} > {times_folder}/{out_name} 2>&1")
