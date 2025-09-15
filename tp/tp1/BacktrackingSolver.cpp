#include "BacktrackingSolver.h"
#include <iostream>
#include <algorithm>
#include <climits>

BacktrackingSolver::BacktrackingSolver() {}

Solution BacktrackingSolver::solve(const Instance& instance, Solution& sol_parcial, int i, Solution& bestSolution) {
    int bestCost = bestSolution.getCost(); //busco el costo total de la mejor solucion hasta ahora
    //Caso base
    if (i == instance.getNumInfluencers()) { //recorrí todos los influencers
        if (sol_parcial.getCoveredCount() == instance.getNumSegments()){ //la solucion cubre los N segmentos
            return sol_parcial;
        }else{ //esa rama no sirve
            Solution vacia = Solution();
            vacia.setCost(INT_MAX); //seteo costo infinito a rama a descartar pues infinito nunca será minimo
            return vacia;
        }
    //podas:
    }else if((sol_parcial.getCost() >= bestCost)){ //Optimalidad: si esta sol tiene MAYOR costo total que la mejor solución vista hasta ahora corto
        return bestSolution; //bestSolution sigue siendo la mejor hasta ahora
    }else if (sol_parcial.getCoveredCount() == instance.getNumSegments()){ //Factibilidad: si ya cubrí los N segmentos corto esta rama
        return sol_parcial;
    }
    //Caso recursivo
    int costoInfluencer = instance.getInfluencer(i).first;
    const set<int>& segments = instance.getInfluencer(i).second;
    
    sol_parcial.addInfluencer(i, costoInfluencer, segments);
    Solution with = solve(instance, sol_parcial, i+1, bestSolution); //agrego al influencer i 
    
    if (with.getCost() < bestSolution.getCost()){ //tras agregar tengo costo menor que la mejor sol hasta ahora?
        bestSolution = with;
    }

    sol_parcial.removeInfluencer(i, costoInfluencer, segments);
    Solution without = solve(instance, sol_parcial, i+1, bestSolution); //no lo agrego
    
    if (without.getCost() < bestSolution.getCost()){
        bestSolution = without;
    }

    return bestSolution;

}
