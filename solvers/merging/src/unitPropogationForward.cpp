#include "unitPropogationForward.h"

#include <cassert>
#include <cstddef>
#include <iostream>

int unitPropogationForward(const Formula& f, const Assignment& assignment)
{
    std::set<int> assignedLiterals(assignment.begin(), assignment.end());
    int unsatisfiedHardClausesWeight = 0;
    int unsatisfiedSoftClausesWeight = 0;

    const auto processClause = [&assignedLiterals](const Clause& c, int& unsatisfiedWeight)
    {
        int unassignedLiteral = 0;
        bool clauseSatisfied = false;
        for (int lit : c.variables)
        {
            if (assignedLiterals.count(lit))
            {
                clauseSatisfied = true;
                break;
            }
            if (assignedLiterals.count(-lit))
            {
                continue;
            }
            assert(lit != 0);
            assert(0 == unassignedLiteral);
            unassignedLiteral = lit;
        }
        if (clauseSatisfied)
        {
            return;
        }
        if (unassignedLiteral == 0)
        {
            unsatisfiedWeight += c.weight;
            return;
        }
        assignedLiterals.insert(unassignedLiteral);
    };

    for (size_t i = 0; i < f.hardClauses.size(); i++)
    {
        const Clause& c = f.hardClauses[i];
        processClause(c, unsatisfiedHardClausesWeight);
    }
    if (unsatisfiedHardClausesWeight > 0)
    {
        return -unsatisfiedHardClausesWeight;
    }
    for (size_t i = 0; i < f.softClauses.size(); i++)
    {
        const Clause& c = f.softClauses[i];
        processClause(c, unsatisfiedSoftClausesWeight);
    }
    return f.softClausesWeight - unsatisfiedSoftClausesWeight;
}