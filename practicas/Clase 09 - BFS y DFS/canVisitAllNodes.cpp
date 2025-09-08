#include <bits/stdc++.h>
using namespace std;

//  Notar que arr es la lista de adyacenciadel nodo i
bool canVisitAllNodes( std::vector<std::vector<int>> & arr, const int & X, const int & n)
{
    std::queue< int > q;
    std::vector< int > visited( n, false );

    //  Hay un nodo aislado (sin vecinos)
    if ( n-1 >= arr.size() ) return false;
    //  No existe el nodo
    if ( X > n ) return false;

    visited[X] = true;
    q.push(X);

    int count = 0;

    while ( q.size() > 0 )
    {
        int size = q.size();
        for ( int i = 0; i < size; i++ )
        {
            auto curr = q.front();
            q.pop();
            count++;
            for ( auto j : arr[curr] )
            {
                if ( visited[j] == false )
                {
                    q.push(j);
                    visited[j] = true;
                }
            }
        }
    }
    return count == n;
}

int main()
{
    std::vector< std::vector< int > > arr = {{1, 2}, {0, 3, 2}, {0, 1}, {1}};
    int N = 4, X = 0;

    if ( canVisitAllNodes(arr, X, N) ) std::cout << "YES" << std::endl;
    else std::cout << "NO" << std::endl;
    return 0;
}
