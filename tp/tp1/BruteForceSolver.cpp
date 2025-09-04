#include "BruteForceSolver.h"
#include <iostream>
#include <algorithm>
#include <climits>

BruteForceSolver::BruteForceSolver() {}

//Funciones auxiliares
namespace {
    bool coversAll(const Solution& sol_parcial, const Instance& instance) {
        set<int> segCub = {};
        set<int> temp = {};
        for(int i=0; i < sol_parcial.InfluencerSize(); i++){ //itero por toda mi sol parcial
            int pos = sol_parcial.getInfluencerPos(i); //busco qué influencer es
            temp = instance.getInfluencer(pos).second; //busco los segmentos que cubre
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

Solution BruteForceSolver::solve(const Instance& instance, Solution sol_parcial, int i) {
    //constructores
    Solution bestSolution = Solution(); //inicializo vector a devolver 
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
    }else{    
        //Caso recursivo
        b = solve(instance, sol_parcial, i+1); //no lo agrego
        int costoInfluencer = instance.getInfluencer(i).first;
        sol_parcial.addInfluencer(i, costoInfluencer);
        a = solve(instance, sol_parcial, i+1); //agrego al influencer i

       
        //Comparo a y b y me quedo con la solucion de menor costo
        bestSolution = minCost(a, b);
        
        return bestSolution;
    }
}