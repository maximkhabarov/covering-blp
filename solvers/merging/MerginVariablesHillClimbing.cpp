#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>
#include <set>
#include <algorithm>
#include <string>
#include <functional>
#include <random>
#include <future>

using namespace std;

struct Clause
{
    int w;
    set<int> variables;
};

struct Formula
{
    vector<Clause> hard;
    vector<Clause> soft;
    int varsCount;
};

Formula readFormula(const string& filename)
{
    Formula f;
    ifstream in(filename);
    string line;
    int clausesCount = -1;
    int top = -1;
    while (getline(in, line))
    {
        if (line[0] == 'c') continue;
        stringstream ss(line);
        if (line[0] == 'p')
        {
            assert(top == -1);
            string p, wcnf;
            ss >> p >> wcnf;
            assert(p == "p");
            assert(wcnf == "wcnf");

            ss >> f.varsCount;
            ss >> clausesCount;
            ss >> top;
        } else {
            clausesCount--;
            assert(top != -1);
            Clause c;
            ss >> c.w;
            const bool is_hard = (c.w == top);
            int lit;
            while (true)
            {
                ss >> lit;
                if (lit == 0) break;
                c.variables.insert(lit);
            }
            if (is_hard) f.hard.push_back(c);
            else f.soft.push_back(c);
        }
        if (clausesCount == 0) break;
    }
    return f;
}


void UnitProp(Formula f, std::vector<int> assignment)
{
    while (assignment.size() > 0)
    {
        int lit = assignment.back();
        for (int i = 0; i < f.hard.size();)
        {
            Clause& c = f.hard[i];
            if (c.variables.count(lit))
            {
                swap(c, f.hard.back());
                f.hard.pop_back();
                continue;
            }
            if (c.variables.count(-lit))
            {
                c.variables.erase(-lit);
            }
        }
    }
}

struct Merging
{
    vector<vector<int>> buckets;
    vector<int> variableToBucket;
};

Merging createMerging(int n, int b, mt19937& rnd)
{
    Merging m;
    m.buckets.resize(b);
    m.variableToBucket.resize(n);

    vector<int> variables;
    for (int i = 0; i < n; i++) variables.push_back(i);
    shuffle(variables.begin(), variables.end(), rnd);

    for (int i = 0; i < n; i++)
    {
        m.buckets[i % b].push_back(variables[i]);
        m.variableToBucket[variables[i]] = i % b;
    }

    return m;
}

struct ComputationResult
{
    vector<int> assignment;
    int targetValue;
};

ComputationResult processCore
(
    const Merging& m
    , const vector<int>& core
    , function<int(const vector<int>&)>& computeTarget
)
{
    ComputationResult scr;
    scr.targetValue = computeTarget(core);
    scr.assignment = core;
    vector<future<ComputationResult>> blocksCompute;
    for (int i = 0; i < m.buckets.size(); i++)
    {
        blocksCompute.emplace_back(std::async(std::launch::async, [&, i](){
            ComputationResult cr = scr;
            for (int mask = 1; mask < (1 << m.buckets[i].size()); mask++)
            {
                vector<int> assignment = core;
                for (int j = 0; j < m.buckets[i].size(); j++)
                {
                    if (mask & (1 << j))
                    {
                        assignment[m.buckets[i][j]] *= -1;
                    }
                }
                int newTarget = computeTarget(assignment);
                if (newTarget > cr.targetValue)
                {
                    cr.targetValue = newTarget;
                    cr.assignment = assignment;
                }
            }
            return cr;
        }));
    }
    for (int i = 0; i < m.buckets.size(); i++)
    {
        const auto res = blocksCompute[i].get();
        if (res.targetValue > scr.targetValue)
        {
            scr = res;
        }
    }
    return scr;
}







int main(int argc, char**argv)
{
    const string filename = argv[1];
    Formula f = readFormula(filename);

    return 0;
}