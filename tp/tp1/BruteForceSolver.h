#ifndef BRUTEFORCE_SOLVER_H
#define BRUTEFORCE_SOLVER_H

#include "Instance.h"
#include "Solution.h"
#include <utility>

class BruteForceSolver {
private:

public:
    BruteForceSolver();
    Solution solve(const Instance& instance, Solution sol_parcial, int i);

    // Resuelve por micro y macro influencers según k
    pair<Solution, Solution> solveMicroMacro(const Instance& instance, int k);
    
    
    
};

#endif // BRUTEFORCE_SOLVER_H


