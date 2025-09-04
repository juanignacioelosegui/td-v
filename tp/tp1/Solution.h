#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "Instance.h"

using namespace std;

class Solution {
    //lista de influencers seleccionados que cubren todos los segmentos con el menor costo
private:
    int _costoTotal; //costo total de la solucion
    vector<int> _influencersUsados; //vector de influencers usados en la solucion

public:
    // Constructor
    Solution();

    void addInfluencer(int influencer, int costo);
    void removeInfluencer(int influencer, int costo);

    void printSolution() const;
    
    void setCost(int costo);
    int getCost() const;
    int getInfluencerPos(int influencer) const;
    int InfluencerSize() const;
};

#endif // SOLUTION_H

