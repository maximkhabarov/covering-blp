#include "exploreNeighbourhood.h"

#include <future>

#include <cassert>

BestInNeighbourhood exploreNeighbourhood
(
    const Merging& m
    , const Assignment& core
    , std::function<int(const Assignment&)> computeTarget
)
{
    BestInNeighbourhood best;
    best.targetValue = computeTarget(core);
    best.assignment = core;
    std::vector<std::future<BestInNeighbourhood>> blocksToCompute;
    for (size_t i = 0; i < m.buckets.size(); i++)
    {
        blocksToCompute.emplace_back(std::async(std::launch::async, [i, &core, &computeTarget, &m](){
            BestInNeighbourhood bestInBlock;
            bestInBlock.targetValue = computeTarget(core);
            bestInBlock.assignment = core;
            for (int mask = 1; mask < (1 << m.buckets[i].size()); mask++)
            {
                Assignment assignment = core;
                assert(m.buckets[i].size() < 30);
                for (size_t j = 0; j < m.buckets[i].size(); j++)
                {
                    if (mask & (1 << j))
                    {
                        assignment[m.buckets[i][j]] *= -1;
                    }
                }
                int newTarget = computeTarget(assignment);
                if (newTarget > bestInBlock.targetValue)
                {
                    bestInBlock.targetValue = newTarget;
                    bestInBlock.assignment = assignment;
                }
            }
            return bestInBlock;
        }));
    }
    for (size_t i = 0; i < m.buckets.size(); i++)
    {
        const auto res = blocksToCompute[i].get();
        if (res.targetValue > best.targetValue)
        {
            best = res;
        }
    }
    return best;
}