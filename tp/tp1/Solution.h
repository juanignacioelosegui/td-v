#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <numeric>
#include "Instance.h"
using namespace std;

class Solution
{
private:
    map<int, pair<int, set<int>>> _solution;

public:
    //  Constructor
    explicit Solution( int numInfluencers );

    //  Modificadores
    void addInfluencer( int influencer, const Instance & inst );
    void removeInfluencer( int influencer );
    void clear();

    //  Observadores
    bool containsInfluencer( int influencer ) const;
    int size() const;

    //  Calculos
    int totalCost() const;
    set<int> coveredSegments() const;
    bool coversAll( const Instance & inst ) const;
    set<int> uncoveredSegments( const Instance & inst ) const;
    vector<int> chosenInfluencers() const;

    //  Print
    void printSolution() const;
};

#endif