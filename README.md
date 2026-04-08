Here is a shorter, cleaner version of the README.

```markdown
# BLP Problem Solvers

Comparison of different solvers for the **Branch Location Problem (BLP)**.  
Includes instance generators, greedy / merging-based heuristics, SCIP, and Loandra (MAX-SAT solver).

## Repository Structure

```
.
├── cnf-generator/               # Instance generation
│   ├── params.txt               # Parameters (one config per line)
│   ├── generate_params_list.txt # Auto‑generate params.txt
│   ├── blp_generator.py         # Single instance → CNF + ILP
│   ├── generate.py              # Batch generation from params.txt
│   └── run_generate.sh          # Cluster submission script
└── solvers/
    ├── greedy/                  # Greedy heuristic (C++)
    ├── merging/                 # MVHC / PMVHC (C++) [Semenov, MOTOR 2019]
    ├── scip_solver.py           # SCIP wrapper
    ├── run_merging.py           # Wrapper for merging algorithms
    ├── run_loandra.py           # Loandra wrapper
    ├── call_loandra_binary.py   # Loandra with binary search
    └── task_*.sh / run_scip.sh  # Cluster scripts
```

## Requirements

- Python 3.7+, C++17 compiler
- `pyscipopt`, `python-sat`
- Loandra – [github.com/jezberg/loandra](https://github.com/jezberg/loandra)

## Installation

```bash
git clone https://github.com/yourusername/BLP-problem-solvers.git
git clone github.com/jezberg/loandra](https://github.com/jezberg/loandra
pip install pyscipopt python-sat
cd solvers
cd merging && make
cd greedy && g++ -O2 main.cpp -o main
```

## Generating Instances

```bash
# Single instance
python blp_generator.py

# Batch from params.txt
python generate.py

# Cluster
./run_generate.sh
```

## Solvers

| Solver          | Command |
|----------------|---------|
| Greedy          | `./greedy instance.cnf` |
| MVHC / PMHC     | `./mvhc instance.cnf` / `./pmhc instance.cnf` |
| SCIP            | `python scip_solver.py --ilp instance.lp` |
| Loandra         | `python run_loandra.py --cnf instance.cnf` |


## Running on a Cluster

Edit and submit the provided `*.sh` scripts (SLURM/GridEngine):

```bash
sbatch task_loandra.sh
sbatch task_merging.sh
sbatch run_scip.sh
```

## License

MIT
```

This version is about one third of the original length, retains all essential information, and removes the table of contents, detailed citation, contributing guide, and lengthy explanations.
