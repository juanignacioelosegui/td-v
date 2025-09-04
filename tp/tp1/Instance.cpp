#include "Instance.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

using namespace std;

Instance::Instance() : N(0), M(0) {
    _instancia = {};
}

bool Instance::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return false;
    }
    
    // Leer numero de segmentos
    file >> N;
    if (N <= 0) {
        cout << "Error: Invalid number of segments" << endl;
        return false;
    }
    
    // Leer numero de influencers
    file >> M;
    if (M <= 0) {
        cout << "Error: Invalid number of influencers" << endl;
        return false;
    }
    
    // Leer informacion de los influencers
    string line;
    getline(file, line);
    
    _instancia.clear();
    for (int i = 0; i < M; i++) {
        getline(file, line);
        stringstream ss(line);
        string token;
        set<int> segments;
        int cost;
        // Leer costo (primer elemento)
        if (getline(ss, token, ',')) {
            cost = stoi(token);
        } else {
            return false;
        }
        
        // Leer segmentos (elementos restantes)
        while (getline(ss, token, ',')) {
            int segment = stoi(token);
            segments.insert(segment);
        }
        _instancia.push_back({cost, segments});
    }
    
    file.close();
    return true;
}

int Instance::getNumSegments() const {
    return N;
}

int Instance::getNumInfluencers() const {
    return M;
}

const pair<int, set<int>>& Instance::getInfluencer(int influencer) const {
    return _instancia[influencer];
}

void Instance::printInstance() const {
    cout << "Numero de segmentos: " << N << "\n";
    cout << "Numero de influencers: " << M << "\n";

    for (int i = 0; i < (int)_instancia.size(); i++) {
        const pair<int, set<int>>& data = _instancia[i];
        int costo = data.first;
        const set<int>& segmentos = data.second;

        cout << "Influencer " << i << ": Costo = " << costo << ", Segmentos = {";
        bool first = true;
        for (int seg : segmentos) {
            if (!first) cout << ", ";
            cout << seg;
            first = false;
        }
        cout << "}\n";
    }
}