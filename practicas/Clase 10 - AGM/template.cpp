#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct DSU
{
    std::vector< int > id;
    DSU( int n )
    {
        id.resize( n );
        for ( int i = 0; i < n; i++ )
        {
            id[i] = i;
        }
    }

    void conectar_nodos( int v, int w )
    {
        v = representante( v );
        w = representante( w );
        if ( v == w ) return;
        id[w] = v;
    }

    bool estan_en_la_misma_componente( int v, int w )
    {
        return representante( v ) == representante( w );
    }
    
    int representante( int v )
    {
        if ( id[v] == v ) return v;
        id[v] = representante( id[v] );
        return id[v];
    }
};

struct Arista
{
    int v, w, costo;
    bool operator<( Arista b )
    {
        return costo < b.costo;
    }
};

int distribucion_de_agua( int n, vector< int > p, vector< vector< int > > c)
{
    /*
     * n es la cantidad de casas
     * p[i] es el costo de contruir un pozo en la casa i
     * c[i][j] es el costo de construir una cañeria entre i y j
     */
    
    //  Actualizamos las aristas (de cañerías y de pozos)
    std::vector<Arista> aristas;

    //  Unir TODAS las cañerías con las casas
    //      Casa origen (i) -> Casa destino (j)
    for ( int i = 0; i < n; i++ )
    {
        for ( int j = 0; j < n; j++ )
        {
            aristas.push_back( {i, j, c[i][j]} );
        }
    }

    //  Conectar los costos de poner un pozo en la casa i
    for (int i = 0; i < n; i++)
    {
        aristas.push_back( {i, n, p[i]} );
    }

    //  Nos va a facilitar ordenar las aristas antes de Kruskal
    sort( aristas.begin( ), aristas.end( ) );

    //  Hacemos DSU con las casas MÁS el pozo que agregamos
    DSU dsu( n + 1 );
    int costo_total = 0;

    for ( auto a : aristas )
    {
        if ( !dsu.estan_en_la_misma_componente( a.v, a.w ) )
        {
            dsu.conectar_nodos( a.v, a.w );
            costo_total += a.costo;
        }
    }
    return costo_total;
}

int main()
{
    //  Ejemplo
    std::cout << distribucion_de_agua(3, {1, 2, 2}, {{0, 1, 2}, {1, 0, 3}, {2, 3, 0}}) << std::endl;
}
