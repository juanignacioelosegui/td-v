#ifndef INSTANCE_H
#define INSTANCE_H

#include <map>
#include <vector>
#include <string>
#include <set>
using namespace std;

class Instance {
private:
    int N; // Numero de segmentos
    int M; // Numero de influencers
   
    //Decidir estructura de instancia
    // o armarlo con map
    map<int, pair<int, set<int>>> _instancia; //todos los influencers 
    //el influencer i mapea a su costo y conjunto de segmentos

public:
    // Constructor
    Instance();
    
    // Cargar instancia desde archivo
    bool loadFromFile(const std::string& filename);
    
    // Getters
    int getNumSegments() const;
    int getNumInfluencers() const;

    bool hasInfluencer(int influencer) const;
    const pair<int, set<int>>& getInfluencer(int influencer) const;
    
    // Mostrar informacion de la instancia
    void printInstance() const;

};

#endif // INSTANCE_H
