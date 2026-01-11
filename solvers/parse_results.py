import os


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

loandra_time_results = {}
loandra_stations_results = {}

merging_times_results = {}
merging_stations_results = {}

params_list = []

merging_time_results = {}
merging_stations_results = {}

mergings_list = [(4,10), (4,100), (4,16), (4,4), (8,8)]

for merge in mergings_list:
    merging_stations_results[merge] = dict()
    merging_time_results[merge] = dict()

with open("params.txt") as params:
    for l in params.readlines():
        l = l.strip()
        for test_num in range(10):
            params_list.append([str(test_num)] + l.split())

for params in params_list:
    a1, a2, a3, a4, a5, a6, a7, a8 = params
    results_file = 'loandra_results/res_' + f'test_{a1}_s{a2}_c{a3}_radius{a4}_SClientsLBound{a5}_seed{a6}_x{a7}_y{a8}.txt'
    assert os.path.exists(results_file)
    times_file = 'loandra_times/' + '_'.join(params) + '.txt'
    assert os.path.exists(times_file)
    output_name = '_'.join(params)
    with open(results_file) as f:
        for l in f.readlines():
            if l.startswith("Minimum number of stations required:"):
                loandra_stations_results[output_name] = int(l.split(':')[1].strip())
    with open(times_file) as f:
        for l in f.readlines():
            if 'elapsed' in l:
                loandra_time_results[output_name] =l.split('elapsed')[0].split()[-1]
for merge in mergings_list:
    for params in params_list:
        a1, a2, a3, a4, a5, a6, a7, a8 = params
        results_file = f'merging_logs_block{merge[0]}_restart{merge[1]}/' + f'test_{a1}_s{a2}_c{a3}_radius{a4}_SClientsLBound{a5}_seed{a6}_x{a7}_y{a8}.txt'
        assert os.path.exists(results_file)
        times_file = f'merging_times_block{merge[0]}_restart{merge[1]}/' + '_'.join(params) + f'_{merge[0]}_{merge[1]}.txt'
        assert os.path.exists(times_file)
        output_name = '_'.join(params)

        with open(results_file) as f:
            for l in f.readlines():
                if l.startswith("Minimum number of stations required:"):
                    merging_stations_results[merge][output_name] = int(l.split(':')[1].strip())

        with open(times_file) as f:
            for l in f.readlines():
                if 'elapsed' in l:
                    merging_time_results[merge][output_name] =l.split('elapsed')[0].split()[-1]

def convert_time_to_seconds(t):
    x, y = t.split(':')
    return int(x) * 60 + int(float(y))

print('                                | loandra', end = '      ')
for merge in mergings_list:
    print(merge, end = '      ')

print('\n-------------------------------------------------------------------------------------------------------------------')

differences = dict()
time_differences = dict()
one_differences = dict()
for merge in  mergings_list: time_differences[merge] = 0
for merge in  mergings_list: one_differences[merge] = 0
total_comparison = 0

for params in params_list:
    output_name = '_'.join(params)
    loandra_result = loandra_stations_results[output_name] if output_name in loandra_stations_results else '-'
    loandra_time = convert_time_to_seconds(loandra_time_results[output_name])
    
    print(f"{output_name:32}|", end=' ')
    print(f'({loandra_result:3}; {loandra_time:4})',end=', ')
    for merge in mergings_list:
        stations_results = merging_stations_results[merge][output_name] if output_name in merging_stations_results[merge] else '-' 
        time_results = convert_time_to_seconds(merging_time_results[merge][output_name])
        result_worse = False
        time_worse = False
        if stations_results != loandra_result and loandra_result != '-':
            if merge in differences:
                differences[merge] += 1
            else:
                differences[merge] = 1
            result_worse = True
        if time_results > loandra_time:
            if merge in time_differences:
                time_differences[merge] += 1
            else:
                time_differences[merge] = 1
            time_worse = True
        text = f"({stations_results:3}; {time_results:4}), "

        if time_worse and result_worse:
            print(bcolors.FAIL+text+bcolors.ENDC,end=' ')
            one_differences[merge] += 1
        elif time_worse or result_worse:
            print(bcolors.WARNING+text+bcolors.ENDC,end=' ')
            one_differences[merge] += 1
        else:
            print(text, end=' ')
    total_comparison += 1
    print()

print('-------------------------------------------------------------------------------------------------------------------')

print(total_comparison,end='                             |               ')
for merge in mergings_list:
    print(f'{one_differences[merge]:<5}',end='         ')
print()
