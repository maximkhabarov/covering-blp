#pragma once

#include "Merging.h"

#include <random>

Merging createUniformMerging(int itemsCount, int bucketsCount, std::mt19937& rnd);