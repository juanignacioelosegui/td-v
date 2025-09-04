#include "BacktrackingSolver.h"
#include <iostream>
#include <algorithm>
#include <climits>

//Funciones auxiliares
namespace { //para evitar problemas de compilación
    bool coversAll(const Solution& sol_parcial, const Instance& instance) {
        set<int> segCub = {};
        for(int i=0; i < sol_parcial.InfluencerSize(); i++){
            int pos = sol_parcial.getInfluencerPos(i);
            set<int> temp = instance.getInfluencer(pos).second;
            segCub.insert(temp.begin(), temp.end());
        }
        return (int)segCub.size() == instance.getNumSegments();
    }

    Solution minCost(Solution a, Solution b){
        if(a.getCost() < b.getCost()){
            return a;
        }else{
            return b;
        } 
    }
} //namespace

BacktrackingSolver::BacktrackingSolver() {}

Solution BacktrackingSolver::solve(const Instance& instance, Solution sol_parcial, int i, Solution bestSolution) {
    //constructores 
    int bestCost = bestSolution.getCost(); //busco el costo total de la mejor solucion hasta ahora
    Solution a = Solution();
    Solution b = Solution();
    //Caso base
    if (i == instance.getNumInfluencers()) { //recorrí todos los influencers
        if(coversAll(sol_parcial, instance) == true){ //la solucion cubre los N segmentos
            return sol_parcial;
        }else{ //esa rama no sirve
            Solution vacia = Solution();
            vacia.setCost(INT_MAX); //seteo costo infinito a rama a descartar pues infinito nunca será minimo
            return vacia;
        }
    //podas:
    }else if((sol_parcial.getCost() >= bestCost)){ //Optimalidad: si esta sol tiene MAYOR costo total que la mejor solución vista hasta ahora corto
        return bestSolution; //bestSolution sigue siendo la mejor hasta ahora
    }else if(coversAll(sol_parcial, instance)){ //Factibilidad: si ya cubrí los N segmentos corto esta rama
        return sol_parcial;
    }
    //Caso recursivo
    b = solve(instance, sol_parcial, i+1, bestSolution); //no lo agrego

    Solution bestAfterB = minCost(b, bestSolution); //si b ya mejoró al bestSolution, poda más eficiente
    int costoInfluencer = instance.getInfluencer(i).first;
    sol_parcial.addInfluencer(i, costoInfluencer);
    a = solve(instance, sol_parcial, i+1, bestAfterB); //agrego al influencer i
    
    //Comparo a y b y me quedo con la solucion de menor costo
    return minCost(a, b);

}