#ifndef INSTANCE_H
#define INSTANCE_H

#include <map>
#include <vector>
#include <string>
#include <set>

using namespace std;

class Instance {
    //lista de todos los influencers con su info completa
private:
    int N; // Numero de segmentos
    int M; // Numero de influencers
   
    //estructura de instancia
    vector<pair<int, set<int>>> _instancia; //todos los influencers 
    //cada pos i es un influencer donde encuentro su costo y conjunto de segmentos que cubre

public:
    // Constructor
    Instance();
    
    // Cargar instancia desde archivo
    bool loadFromFile(const std::string& filename);
    
    int getNumSegments() const;
    int getNumInfluencers() const;

    const pair<int, set<int>>& getInfluencer(int influencer) const;

    // Mostrar informacion de la instancia
    void printInstance() const;

};

#endif // INSTANCE_H
