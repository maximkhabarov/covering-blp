import os

with open("params.txt") as params_list:
    l = params_list.readlines()
    for i, params in enumerate(l):
        stations, clients, radius, sclients, seed, x, y = params.split()
        os.system(f"python3 blp_generator.py -s {stations} -c {clients} -x {x} -y {y} -r {radius} -n 1 -ucb {int(clients) - int(sclients)} -sd {seed}")
        print(f"generated {i + 1} out of {len(l)}")
