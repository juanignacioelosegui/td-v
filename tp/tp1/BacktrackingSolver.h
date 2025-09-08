#ifndef BACKTRACKING_SOLVER_H
#define BACKTRACKING_SOLVER_H

#include "Instance.h"
#include "Solution.h"
#include <set>
#include <utility>

class BacktrackingSolver {
private:
  

public:
    BacktrackingSolver();
    Solution solve(const Instance& instance, Solution& sol_parcial, int i, Solution& bestSolution);

    // Resuelve por micro y macro influencers según k
    //pair<Solution, Solution> solveMicroMacro(const Instance& instance, int k);
};

#endif // BACKTRACKING_SOLVER_H
