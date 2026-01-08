#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>
#include <string>

using namespace std;

struct Clause
{
    int w;
    vector<int> variables;
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
                c.variables.push_back(lit);
            }
            if (is_hard) f.hard.push_back(c);
            else f.soft.push_back(c);
        }
        if (clausesCount == 0) break;
    }
    return f;
}

int main(int argc, char**argv)
{
    const string filename = argv[1];
    readFormula("C:\Users\\Maxim\\work\\Yandex.Disk\\itmo\\research\\covering-blp\\cnf-generator\\WCNFs\\test_0_10_100_5_radius2-4_SClientsLBound100_l4.wcnf";)
    return 0;
}