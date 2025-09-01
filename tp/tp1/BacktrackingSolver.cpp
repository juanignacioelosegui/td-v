#include "BacktrackingSolver.h"
#include <iostream>
#include <algorithm>
#include <climits>

BacktrackingSolver::BacktrackingSolver() {}

Solution BacktrackingSolver::solve(const Instance& instance){
    Solution bestSolution(instance.getNumInfluencers());
    return bestSolution;
}
