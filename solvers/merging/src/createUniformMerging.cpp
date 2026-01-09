#include "createUniformMerging.h"

#include <algorithm>

Merging createUniformMerging(int n, int b, std::mt19937& rnd)
{
    Merging m;
    m.buckets.resize(b);
    m.itemToBucket.resize(n);

    std::vector<int> items;
    for (int i = 0; i < n; i++) items.push_back(i);
    std::shuffle(items.begin(), items.end(), rnd);

    for (int i = 0; i < n; i++)
    {
        m.buckets[i % b].push_back(items[i]);
        m.itemToBucket[items[i]] = i % b;
    }

    return m;
}