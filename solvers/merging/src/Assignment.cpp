#include "Assignment.h"

#include <cstddef>
#include <fstream>
#include <iostream>

int positiveLiterals(const Assignment& assignment)
{
    int res = 0;
    for (size_t i = 0; i < assignment.size(); i++)
    {
        if (assignment[i] > 0)
        {
            res++;
        }
    }
    return res;
}

void print(std::ofstream &of, const Assignment& assignment)
{
    for (int x : assignment)
    {
        of << x << " ";
    }
    of << std::endl;
}