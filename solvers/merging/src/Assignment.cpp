#include "Assignment.h"

#include <cstddef>
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

void print(const Assignment& assignment)
{
    for (int x : assignment)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}