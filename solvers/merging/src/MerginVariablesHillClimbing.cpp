#include "exploreNeighbourhood.h"
#include "readWcnf.h"
#include "unitPropogationForward.h"
#include "createUniformMerging.h"

#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>

int main(int argc, char**argv)
{
    assert(12 == argc);

    const size_t test_number = std::stoi(argv[1]);
    const size_t stations_number = std::stoi(argv[2]);
    const size_t clients_number = std::stoi(argv[3]);
    const std::string radius = argv[4];
    const size_t satisfied_clients_lower_bound = std::stoi(argv[5]);
    const size_t seed = std::stoi(argv[6]);
    const size_t x = std::stoi(argv[7]);
    const size_t y = std::stoi(argv[8]);
    const size_t blockSize = std::stoi(argv[9]);
    size_t merging_restarts_left = std::stoi(argv[10]);
    const std::string output_folder = argv[11];

    const size_t unsatisfied_clients_upper_bound = clients_number - satisfied_clients_lower_bound;

    std::mt19937 rnd(239);

    std::string subfolder_name =
            "test_"
            + std::to_string(test_number)
            + "_s"
            + std::to_string(stations_number)
            + "_c"
            + std::to_string(clients_number)
            + "_radius"
            + radius
            + "_SClientsLBound"
            + std::to_string(satisfied_clients_lower_bound)
            + "_seed"
            + std::to_string(seed)
            + "_x"
            + std::to_string(x)
            + "_y"
            + std::to_string(y);

    std::ofstream of(output_folder + "/" + subfolder_name + ".txt");
    assert(not of.fail());

    size_t L = 0, R = stations_number;
    while (R - L > 1)
    {
        size_t M = (L + R) / 2;
        std::string folder = "../cnf-generator/WCNFs/";
        folder += subfolder_name + "/";
        std::string filename = std::to_string(M) + ".wcnf";
        of << "Stations upper bound: " << M << std::endl;
        Formula f = readFormula(folder + filename);
        of << "core: ";
        for (int x : f.coreVariables) of << x << " ";
        of << std::endl;
        of << "hard clauses count: " << f.hardClauses.size() << std::endl;
        of << "soft clauses count: " << f.softClauses.size() << std::endl;
        of << "hard clauses weight: " << f.hardClausesWeight << std::endl;
        of << "soft clauses weight: " << f.softClausesWeight << std::endl;

        Assignment init_assignment;
        for (size_t i = 0; i < f.coreVariables.size(); i++)
        {
            init_assignment.push_back(f.coreVariables[i]);
            if ((rnd() % f.coreVariables.size()) >= M)
            {
                init_assignment.back() *= -1;
            }
        }
        of << "start target value: " << unitPropogationForward(f, init_assignment) << std::endl;
        of << "stations turned: " << positiveLiterals(init_assignment) << std::endl;

        BestInNeighbourhood r;
        r.assignment = init_assignment;
        r.targetValue = unitPropogationForward(f, init_assignment);

        Merging merging = createUniformMerging(f.coreVariables.size(), f.coreVariables.size() / blockSize, rnd);

        while (true)
        {
            const auto res = exploreNeighbourhood(merging, r.assignment, [&f, M](const Assignment& assignment){
                return unitPropogationForward(f, assignment);
            });
            of << "Target value: " << res.targetValue << std::endl;
            of << "Stations turned: " << positiveLiterals(res.assignment) << std::endl;
            of << "Assignment: ";
            print(of, res.assignment);

            if (r.targetValue < res.targetValue)
            {
                r = res;
            } else {
                if (merging_restarts_left > 0)
                {
                    merging = createUniformMerging(f.coreVariables.size(), f.coreVariables.size() / blockSize, rnd);
                    merging_restarts_left--;
                }
                else{
                    break;
                }
            }
        }

        if (r.targetValue >= (int)(clients_number - unsatisfied_clients_upper_bound))
        {
            of << M << " Satisfied" << std::endl;
            R = M;
        }
        else {
            of << M << " Unsatisfied" << std::endl;
            L = M;
        }
    }

    of << "Result for problem: " << subfolder_name << std::endl;
    of << "Minimum number of stations required: " << R << std::endl;

    return 0;
}