import os
import sys

test_number = int(sys.argv[1])
stations_number = int(sys.argv[2])
clients_number = int(sys.argv[3])
radius = sys.argv[4]
satisfied_clients_lower_bound = int(sys.argv[5])
seed = int(sys.argv[6])
x = int(sys.argv[7])
y = int(sys.argv[8])
subfolder_name = f"test_{test_number}_s{stations_number}_c{clients_number}_radius{radius}_SClientsLBound{satisfied_clients_lower_bound}_seed{seed}_x{x}_y{y}"

def try_bound(stations_bound):
    folder = "../cnf-generator/WCNFs/"
    file_name = "/" + str(stations_bound) + ".wcnf"
    path = folder + subfolder_name + file_name
    assert os.path.exists(path)

    out_file = f"loandra_logs/{subfolder_name}_{stations_bound}.txt"

    os.system(f"./loandra/loandra -old-format {path} > {out_file}")

    solved = False

    with open(out_file) as f:
        for line in reversed(f.readlines()):
            if line.startswith("o"):
                if int(line.split()[1]) <= clients_number - satisfied_clients_lower_bound:
                    solved = True
                    break

    return solved

with open(f"loandra_results/res_{subfolder_name}.txt", "w") as f:
    f.write("Solving started\n")
    f.flush()

    L = 0
    R = stations_number

    while R - L > 1:
        stations_bound = (L + R) // 2

        f.write(f"Solve for bound {stations_bound}\n")
        f.flush()

        if try_bound(stations_bound):
            R = stations_bound
        else:
            L = stations_bound

    f.write("Result for problem:" + subfolder_name + "\n")
    f.flush()

    if not try_bound(R):
        f.write("Minimum number of stations required: Unsat\n")
    else:
        f.write("Minimum number of stations required:" + str(R) + "\n")
    f.flush()