from pyscipopt import Model
import os
import time

for params in os.listdir("ILPs"):
    print(params)
    model = Model()
    model.readProblem("ILPs/" + params)
    start_time = time.perf_counter() 
    # The model is now loaded and can be optimized
    model.optimize()
    end_time = time.perf_counter()   
    with open("ILPs-results/" + params, "w") as f:
        f.write("Optimal value: " + str(model.getObjVal()) + "\n")
        elapsed_time = end_time - start_time
        f.write(f"Elapsed time: {elapsed_time:.4f} seconds")
