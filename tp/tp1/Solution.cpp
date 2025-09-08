#include "Solution.h"
#include <iostream>
#include <algorithm>
#include <set>

using namespace std;

Solution::Solution() {
    _costoTotal = 0; //inicializo el costo total en 0
    _influencersUsados = {}; //inicializo el vector de influencers vacío, no tengo ningun influencer
    _segmentosCount = {};
    _coveredCount = 0;
}

void Solution::addInfluencer(int influencer, int costo, const set<int>& segmentos) {
    _influencersUsados.push_back(influencer);
    _costoTotal = _costoTotal + costo;
    for (int s : segmentos) {
        if (_segmentosCount[s] == 0){ //si ningún influencer lo cubría
             _coveredCount++;
        }
        _segmentosCount[s]++; //aumento en 1 cuántos influencers lo cubren ahora
    }
}

void Solution::removeInfluencer(int influencer, int costo, const set<int>& segmentos) {
     _costoTotal = _costoTotal - costo;
    if (!_influencersUsados.empty() && _influencersUsados.back() == influencer) {
        _influencersUsados.pop_back();
    } else { //elimino al influencer
        auto it = find(_influencersUsados.begin(), _influencersUsados.end(), influencer);
        if(it != _influencersUsados.end()){
            _influencersUsados.erase(it);
        }
        for (int s : segmentos) {
            _segmentosCount[s]--; //elimino todos los segmentos que cubra
            if (_segmentosCount[s] == 0){ //si era el único que cubría
                _coveredCount--;
            }
        }
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

void Solution::initCover(int N) {
    _segmentosCount.assign(N, 0);
    _coveredCount = 0;
}

int Solution::getCoveredCount() const{
    return _coveredCount;
}
 