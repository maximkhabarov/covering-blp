#pragma once

#include <vector>
#include <fstream>

using Assignment = std::vector<int>;

int positiveLiterals(const Assignment& assignment);
void print(std::ofstream &of, const Assignment& assignment);