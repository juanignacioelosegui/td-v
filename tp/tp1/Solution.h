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
    vector<int> _segmentosCount; //vector de contadores de cuántos influencers cubren cada segmento (pos)
    int _coveredCount; //cuantos segmentos va cubriendo la solucion
public:
    // Constructor
    Solution();

    void addInfluencer(int influencer, int costo, const set<int>& segmentos);
    void removeInfluencer(int influencer, int costo, const set<int>& segmentos);

    void printSolution() const;
    
    void setCost(int costo);
    int getCost() const;
    int getInfluencerPos(int influencer) const;
    int InfluencerSize() const;
    int getCoveredCount() const;

    void initCover(int N);

};

#endif // SOLUTION_H

