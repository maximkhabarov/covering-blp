#include "readWcnf.h"

#include <fstream>
#include <iostream>
#include <cassert>
#include <sstream>

Formula readFormula(const std::string& filename)
{
    Formula f;
    std::ifstream in(filename);
    assert(not in.fail());
    std::string line;
    int clausesLeftCount = -1;
    int top = -1;
    f.hardClausesWeight = 0;
    f.softClausesWeight = 0;
    while (getline(in, line))
    {
        if (line[0] == 'c')
        {
            if (line[2] == 'c')
            {
                std::stringstream ss(line.substr(7));
                int var;
                while (ss >> var)
                {
                    f.coreVariables.push_back(var);
                }
            }
            continue;
        }
        std::stringstream ss(line);
        if (line[0] == 'p')
        {
            assert(top == -1);
            std::string p, wcnf;
            ss >> p >> wcnf;
            assert(p == "p");
            assert(wcnf == "wcnf");

            ss >> f.variablesCount;
            ss >> clausesLeftCount;
            ss >> top;
        } else {
            clausesLeftCount--;
            assert(top != -1);
            Clause c;
            ss >> c.weight;
            const bool is_hard = (c.weight == top);
            int lit;
            while (true)
            {
                ss >> lit;
                if (lit == 0) break;
                c.variables.insert(lit);
            }
            if (is_hard) {
                c.weight = 1;
                f.hardClausesWeight += c.weight;
                f.hardClauses.push_back(c);
            }
            else {
                f.softClausesWeight += c.weight;
                f.softClauses.push_back(c);
            }
        }
        if (clausesLeftCount == 0) break;
    }
    return f;
}
