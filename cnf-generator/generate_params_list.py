with open("params.txt", "w") as f:
    for stations in range(10, 60, 15):
        for clients in range(10000, 100000, 20000):
            for uclients in range(int(clients * 0.1), clients, int(clients * 0.3)):
                for radius_lower_bound in range(10, 30, 5):
                    for radius_upper_bound in range(radius_lower_bound, 30, 5):
                        f.write(f"{stations} {clients} {radius_lower_bound}-{radius_upper_bound} {uclients} 0 50 50\n")
