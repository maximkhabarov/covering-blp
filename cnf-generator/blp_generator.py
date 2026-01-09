import time
import sys
import random
import argparse
import math
import os
import functools


print = functools.partial(print, flush=True)

#Parser
def createParser ():
    parser = argparse.ArgumentParser()
    #python3.8 FLP_test_generator.py -s 357 -c 1418601 -x 50 -y 50 -b 1307449 -csv 1 -r 2-3 -n 1
    parser.add_argument ('-s', '--nof_stations', nargs='?', type=int, default=10, help='Число базовых станций')
    parser.add_argument ('-c', '--nof_clients', nargs='?', type=int, default=1000, help='Число клиентов')
    parser.add_argument ('-x', '--max_x', nargs='?', type=int, default=5, help='Максимум по оси X для сетки')
    parser.add_argument ('-y', '--max_y', nargs='?', type=int, default=5, help='Максимум по оси Y для сетки')
    parser.add_argument ('-r', '--radius', nargs='?', type=str, default='2-4', help='Диапазон радиуса для станцией (через дефис, для каждой станции будет выбрано случайное значение из диапазона)')
    parser.add_argument ('-n', '--number_of_tests', nargs='?', type=int, default=1, help='Сколько тестов делать')
    parser.add_argument ('-ucb', '--unsatisfied_clients_bound', nargs='?', type=int, default=10, help='')
    parser.add_argument ('-sd', '--seed', nargs='?', type=int, default=0, help='')
    return parser

def create_station(number,max_x,max_y,radius):
    coord_x = random.randint(0,max_x-1)
    coord_y = random.randint(0,max_y-1)
    rad1 = int(radius.split('-')[0])
    rad2 = int(radius.split('-')[1])
    radius = random.uniform(rad1,rad2)
    station = (number,coord_x,coord_y,radius)
    return station

def create_client(number,max_x,max_y):
    coord_x = random.uniform(0,max_x-1)
    coord_y = random.uniform(0,max_y-1)
    client = (number,coord_x,coord_y)
    return client

def create_stations(nof_stations,max_x,max_y,radius):
    list_of_stations = []
    set_stations_ = set()
    i = 0
    while len(list_of_stations) < nof_stations:
        new_station = create_station(i,max_x,max_y,radius)
        if (new_station[1],new_station[2]) in set_stations_:
            continue
        else:
            list_of_stations.append(new_station)
            set_stations_.add((new_station[1],new_station[2]))
            i += 1
    return list_of_stations

def create_clients(nof_clients,max_x,max_y):
    list_of_clients = []
    for i in range(nof_clients):
        list_of_clients.append(create_client(i,max_x,max_y))
    return list_of_clients

def compute_distance(station, client):
    dx = station[1] - client[1]
    dy = station[2] - client[2]
    return math.sqrt(dx*dx + dy*dy)

def create_graph(list_of_clients, list_of_stations, clients_enough):
    graph = dict()
    for client in list_of_clients:
        list_of_stations_for_that_client = []
        for station in list_of_stations:
            radius = station[3]
            station_number = station[0]
            distance = compute_distance(station,client)
            if distance <= radius:
                list_of_stations_for_that_client.append(station_number)
        if len(list_of_stations_for_that_client) > 0:
            graph[client[0]] = list_of_stations_for_that_client
        if len(graph) >= clients_enough:
            break
    return graph

def add_clause(d, clause):
    if clause in d:
        d[clause] += 1
    else:
        d[clause] = 1

def le_naive(x, k, f, start_from):
    # s_i_j = x[0] + x[1] + ... x[i - 1] == j

    # a = b <=> (not a or b) and (a or not b)

    # s_0_0 = True <=> (s_0_0)
    # s_0_j, j > 0 = False <=> (-s_0_j)

    # s_i_j = (s_{i - 1}_{j} and not x[i - 1]) or (s_{i - 1}_{j - 1} and x[i - 1])

    def eq_or(x, y, z):
        f.append([-x, y, z])
        f.append([x, -y])
        f.append([x, -z])

    def eq_and(x, y, z):
        f.append([-x, y])
        f.append([-x, z])
        f.append([x, -y, -z])

    n = len(x)
    s = dict()
    for i in range(n + 1):
        for j in range(n + 1):
            s[(i, j)] = start_from
            start_from += 1

    f.append([s[(0, 0)]])
    for j in range(1, n + 1):
        f.append([-s[(0, j)]])

    for i in range(1, n + 1):
        eq_and(s[(i, 0)], s[(i - 1, 0)], -x[i - 1])
        for j in range(1, n + 1):
            h1, h2 = start_from, start_from + 1
            start_from += 2
            eq_and(h1, s[(i - 1, j)], -x[i - 1])
            eq_and(h2, s[(i - 1, j - 1)], x[i - 1])
            eq_or(s[(i, j)], h1, h2)

    for i in range(1, n + 1):
        f.append([s[(i, j)] for j in range(0, k + 1)])
        for j in range(k + 1, n + 1):
            f.append([-s[(i, j)]])
    return start_from

def ge_naive(x, k, f, start_from):
    return le_naive(list(map(lambda y: -y, x)), len(x) - k, f, start_from)

def clause_to_str(clause):
    return ' '.join(map(str, clause)) + ' 0'

def sum_values(d):
    sum = 0
    for key in d:
        sum += d[key]
    return sum

def create_reversed_cnf_clauses(graph, list_of_stations, upper_bound):
    soft_clauses = dict()
    hard_clauses = dict()
    core_variables = list()

    station_variables = dict()

    var_num = 1

    for station in list_of_stations:
        station_variables[station[0]] = var_num
        core_variables.append(var_num)
        var_num += 1

    sinz_clauses = []
    var_num = le_naive(list(station_variables.values()), upper_bound, sinz_clauses, var_num)
    for clause in sinz_clauses:
        add_clause(hard_clauses, clause_to_str(clause))

    for client_number in graph:
        clause = []
        for station_number in graph[client_number]:
            clause.append(station_variables[station_number])
        assert len(clause) > 0
        add_clause(soft_clauses, clause_to_str(clause))

    top = sum_values(soft_clauses) + 1
    for key in hard_clauses:
        hard_clauses[key] = top

    return hard_clauses, soft_clauses, var_num - 1, top, core_variables

def create_folders():
    if not os.path.exists('./WCNFs/'):
        os.makedirs('./WCNFs/')
        print('Directory ./WCNFs/ is created.')

#============================================================================#

#Параметры
start_time = time.time()
parser = createParser()
namespace = parser.parse_args (sys.argv[1:])
nof_stations = namespace.nof_stations
nof_clients = namespace.nof_clients
max_x = namespace.max_x
max_y = namespace.max_y
radius = namespace.radius
nof_tests = namespace.number_of_tests
satisfied_clients_lower_bound = nof_clients - namespace.unsatisfied_clients_bound
seed = namespace.seed

random.seed(seed)

create_folders()

counter = 0
while counter < nof_tests:
    start_test_time = time.time()

    list_of_stations = create_stations(nof_stations,max_x,max_y,radius)
    list_of_clients = create_clients(2 * nof_clients,max_x,max_y)

    graph  = create_graph(list_of_clients, list_of_stations, nof_clients)

    if nof_clients > len(graph):
        print('ERROR Clients is not enough:', len(graph), nof_clients)
        continue

    folder_name = 'test_' + str(counter) + '_s' + str(nof_stations) + '_c' + str(nof_clients) + '_radius' + radius + '_SClientsLBound' + str(satisfied_clients_lower_bound) + "_seed" + str(seed) + "_x" + str(max_x) + "_y" + str(max_y)

    if not os.path.exists("./WCNFs/" + folder_name):
        os.makedirs("./WCNFs/" + folder_name)

    for l in range(1, len(list_of_stations) + 1):
        out_name = str(l)
        out_name_wcnf = './WCNFs/' +folder_name +'/' + out_name + '.wcnf'
        hard_clauses, soft_clauses, vars_num, top, core_variables = create_reversed_cnf_clauses(graph, list_of_stations, l)

        comment = 'c ' + str(nof_stations) + ' ' + str(nof_clients) + ' ' + str(satisfied_clients_lower_bound)
        core_variables = 'c core ' + ' '.join(map(str, core_variables))
        header = 'p wcnf ' + str(vars_num) + ' ' + str(len(hard_clauses) + len(soft_clauses)) + ' ' + str(top)
        with open(out_name_wcnf,'w') as out_file:
            print(comment, file=out_file)
            print(core_variables, file=out_file)
            print(header, file=out_file)
            for key in hard_clauses:
                print(hard_clauses[key],key,file=out_file)
            for key in soft_clauses:
                print(soft_clauses[key],key,file=out_file)
        print('Test', counter, 'WCNF done, total runtime =',time.time()-start_test_time)

    counter += 1

print(nof_tests, 'tests created, total runtime', time.time()-start_time)

# def create_cnf_clauses(graph, list_of_clients, list_of_stations, lower_bound):
#     def eq_or(x, ys):
#         clause_1 = [-x] + ys
#         clauses_2 = [[x, -y] for y in ys]
#         return [clause_1] + clauses_2

#     soft_clauses = dict()
#     hard_clauses = dict()

#     station_variables = dict()
#     client_variables = dict()

#     var_num = 1

#     for station in list_of_stations:
#         station_variables[station[0]] = var_num
#         add_clause(soft_clauses, clause_to_str([-var_num]))
#         var_num += 1

#     for client in list_of_clients:
#         client_variables[client[0]] = var_num
#         var_num += 1
#     for client in list_of_clients:
#         client_number = client[0]
#         for clause in eq_or(client_variables[client_number], [station_variables[station] for station in graph[client_number]]):
#             add_clause(hard_clauses, clause_to_str(clause))

#     client_vars = [client_variables[client[0]] for client in list_of_clients]
#     sinz_clauses = []
#     var_num = ge_naive(client_vars, lower_bound, sinz_clauses, var_num)
#     for clause in sinz_clauses:
#         add_clause(hard_clauses, clause_to_str(clause))
#     top = sum_values(soft_clauses) + 1
#     for key in hard_clauses:
#         hard_clauses[key] = top

#     return hard_clauses, soft_clauses, var_num - 1, top



# if False:
#         out_name = 'test_' + str(counter) + '_' + str(nof_stations)+'_'+str(nof_clients)+'_'+str(min_weights_sum)+'_radius'+radius+'_SClientsLBound'+str(satisfied_clients_lower_bound)
#         out_name_wcnf = './WCNFs/' + out_name + '.wcnf'
#         hard_clauses, soft_clauses, vars_num, top = create_cnf_clauses(graph, list_of_clients, list_of_stations, satisfied_clients_lower_bound)

#         comment = 'c ' + str(nof_stations) + ' ' + str(nof_clients) + ' ' + str(min_weights_sum)
#         header = 'p wcnf ' + str(vars_num) + ' ' + str(len(hard_clauses) + len(soft_clauses)) + ' ' + str(top)
#         with open(out_name_wcnf,'w') as out_file:
#             print(comment, file=out_file)
#             print(header, file=out_file)
#             for key in hard_clauses:
#                 print(hard_clauses[key],key,file=out_file)
#             for key in soft_clauses:
#                 print(soft_clauses[key],key,file=out_file)
#         print('Test', counter, 'WCNF done, total runtime =',time.time()-start_test_time)
#     else: