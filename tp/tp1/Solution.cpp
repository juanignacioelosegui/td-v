#include "Solution.h"
#include <iostream>
#include <algorithm>
#include <set>

using namespace std;

Solution::Solution() {
    _costoTotal = 0; //inicializo el costo total en 0
    _influencersUsados = {}; //inicializo el vector de influencers vacío, no tengo ningun influencer
}

void Solution::addInfluencer(int influencer, int costo) {
    _influencersUsados.push_back(influencer);
    _costoTotal = _costoTotal + costo;
}

void Solution::removeInfluencer(int influencer, int costo) {
    _costoTotal = _costoTotal - costo;
    auto it = find(_influencersUsados.begin(), _influencersUsados.end(), influencer);
    if(it != _influencersUsados.end()){
        _influencersUsados.erase(it);
    }
}

void Solution::printSolution() const {
    cout << "Influencers usados: ";
    for (size_t i = 0; i < _influencersUsados.size(); i++) {
        cout << _influencersUsados[i];
        if (i + 1 < _influencersUsados.size()) {
            cout << " ";
        }
    }
    cout << "\n";
    cout << "Costo total: " << _costoTotal << endl;
}

void Solution::setCost(int costo) {
    _costoTotal = costo;
}

int Solution::getCost() const{
    return _costoTotal;
}

int Solution::getInfluencerPos(int influencer) const{ //devuelve el valor asociado que es el influencer
    return _influencersUsados[influencer];
}

int Solution::InfluencerSize() const{ //cant de influencers para usar en mi sol parcial
    return _influencersUsados.size();
}
