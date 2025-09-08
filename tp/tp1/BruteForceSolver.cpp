#include "BruteForceSolver.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <utility>

BruteForceSolver::BruteForceSolver() {}

//funcion auxiliar
Solution minCost(Solution a, Solution b){
        if(a.getCost() < b.getCost()){
            return a;
        }else{
            return b;
        } 
    }

Solution BruteForceSolver::solve(const Instance& instance, Solution sol_parcial, int i) {
    //Solution bestSolution = Solution();
    //Caso base
    if (i == instance.getNumInfluencers()) { //recorrí todos los influencers
        if (sol_parcial.getCoveredCount() == instance.getNumSegments()){ //la solucion cubre los N segmentos
            return sol_parcial;
        }else{ //esa rama no sirve
            Solution vacia = Solution();
            vacia.setCost(INT_MAX); //seteo costo infinito a rama a descartar pues infinito nunca será minimo
            return vacia;
        }
    }else{
        int costoInfluencer = instance.getInfluencer(i).first;
        const set<int>& segments = instance.getInfluencer(i).second;

        sol_parcial.addInfluencer(i, costoInfluencer, segments);
        Solution with = solve(instance, sol_parcial, i+1); //agrego al influencer i 
        
        sol_parcial.removeInfluencer(i, costoInfluencer, segments);
        Solution without = solve(instance, sol_parcial, i+1);
        
        Solution bestSolution = minCost(without, with);
        return bestSolution;

    }
}