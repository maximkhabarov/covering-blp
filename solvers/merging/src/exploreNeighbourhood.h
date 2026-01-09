#pragma once

#include "Merging.h"
#include "Assignment.h"

#include <vector>
#include <functional>

struct BestInNeighbourhood
{
    Assignment assignment;
    int targetValue;
};

BestInNeighbourhood exploreNeighbourhood
(
    const Merging& m
    , const Assignment& coreAssignment
    , std::function<int(const Assignment&)> computeTarget
);