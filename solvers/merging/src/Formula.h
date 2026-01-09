#pragma once

#include <set>
#include <vector>

struct Clause
{
    int weight;
    std::set<int> variables;
};

struct Formula
{
    std::vector<Clause> hardClauses;
    std::vector<Clause> softClauses;
    int variablesCount;
    std::vector<int> coreVariables;
    int softClausesWeight;
    int hardClausesWeight;
};