from pyscipopt import Model
import os
import time

for params in os.listdir("ILPs"):
    print(params)
    model = Model()
    model.readProblem("ILPs/" + params)
    start_time = time.perf_counter() 
    model.setLogfile("ILPs-results/" + params)
    model.setParam("limits/time", 120)
    model.optimize()
