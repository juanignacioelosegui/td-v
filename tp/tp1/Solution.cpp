#include "Solution.h"
#include <algorithm>
#include <iostream>

Solution::Solution( int /*numInfluencers*/ ) { _solution.clear(); }

void Solution::addInfluencer( int influencer, const Instance & inst ) {
    if ( !containsInfluencer( influencer ) && inst.hasInfluencer( influencer ) )
    { 
        // si NO está en el subconjunto y SÍ es un influencer válido
        _solution.insert({influencer, inst.getInfluencer(influencer)}); 
    } 
    else if ( !inst.hasInfluencer( influencer ) ) std::cout << "El influencer: " << influencer << " no es válido" << std::endl;
    else std::cout << "El influencer: " << influencer << " ya esta en la solución" << std::endl;
}

void Solution::removeInfluencer(int influencer)
{
    auto it = _solution.find(influencer);
    if (it != _solution.end())
    {
        _solution.erase(it);
    } else {
        std::cout << "El influencer " << influencer << " no está en la solución." << std::endl;
    }
}

void Solution::clear() { _solution.clear(); }

bool Solution::containsInfluencer( int influencer ) const
{
    const bool presente = _solution.find( influencer ) != _solution.end();
    if ( presente ) return true; else return false;
}

int Solution::size() const { return _solution.size(); }

int Solution::totalCost() const
{
    int acc = 0;
    for ( const auto & [id, data] : _solution )
    {
        (void)id;
        //  costo
        acc += data.first;
    }
    return acc;
}

set<int> Solution::coveredSegments() const
{
    set<int> uni;
    for ( const auto & [id, data] : _solution )
    {
        ( void )id;
        const set<int> & segs = data.second;
        uni.insert(segs.begin(), segs.end());
    }
    return uni;
}

bool Solution::coversAll( const Instance & inst ) const
{
    //  Asumimos que los segmentos están numerados 1..N (si fuesen otros ids,
    //  igual funciona: comparamos cardinalidad y pertenencia directas)
    const int N = inst.getNumSegments();
    if ( N <= 0 ) return false;

    set<int> cubiertos = coveredSegments();

    //  Chequeo fuerte: que estén TODOS los ids 1..N en la cobertura
    if ( ( int )cubiertos.size() < N ) return false;
    for ( int s = 1; s <= N; s++ )
    {
        //  si encuentra uno solo que no esté
        if ( cubiertos.find( s ) == cubiertos.end() ) return false;
    }
    return true;
}

set<int> Solution::uncoveredSegments( const Instance & inst ) const
{
    set<int> falta;
    const int N = inst.getNumSegments();
    if (N <= 0) return falta;

    set<int> cubiertos = coveredSegments();
    for ( int s = 1; s <= N; s++ )
    {
        //  si se encuentra un segmento no cubierto
        if ( cubiertos.find( s ) == cubiertos.end() ) falta.insert(s);
    }
    return falta;
}

vector<int> Solution::chosenInfluencers() const
{
    vector<int> v;
    v.reserve( _solution.size() );
    for ( const auto & [id, _] : _solution ) v.push_back( id );
    return v;
}

void Solution::printSolution() const
{
    std::cout << "Influencers elegidos: ";
    bool first = true;
    for ( const auto & [id, _] : _solution )
    {
        if ( !first ) std::cout << ", ";
        std::cout << id;
        first = false;
    }
    if ( _solution.empty() ) std::cout << "(ninguno)";
    std::cout << "\n";

    std::cout << "Costo total: " << totalCost() << "\n";

    std::cout << "Segmentos cubiertos: {";
    set<int> cubiertos = coveredSegments();
    first = true;
    for ( int s : cubiertos )
    {
        if ( !first ) std::cout << ", ";
        std::cout << s;
        first = false;
    }
    cout << "}\n";
}