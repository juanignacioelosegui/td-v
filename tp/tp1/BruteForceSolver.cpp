#include "BruteForceSolver.h"
#include <climits>

using namespace std;

namespace BruteForceSolver
{

    //  DFS recursivo sobre decisiones donde idx es el influencer actual (1..M).
    static void dfs( const Instance & inst, int idx, int M, Solution & curr, 
                    int currCost, Solution & best, int & bestCost )
    {
        //  Si ya pasamos el último influencer, evaluamos la solución actual
        if ( idx > M )
        {
            if ( curr.coversAll( inst ) && currCost < bestCost )
            {
                best = curr;
                bestCost = currCost;
            }
            return;
        }

        //  1) NO tomar al influencer idx
        dfs( inst, idx + 1, M, curr, currCost, best, bestCost );

        //  2) Tomar al influencer idx (si existe)
        if ( inst.hasInfluencer( idx ) )
        {
            const auto & info = inst.getInfluencer( idx );
            int costIdx = info.first;

            curr.addInfluencer( idx, inst );
            dfs( inst, idx + 1, M, curr, currCost + costIdx, best, bestCost );
            curr.removeInfluencer( idx );
        }
    }

    Solution solve( const Instance & inst )
    {
        const int M = inst.getNumInfluencers();

        Solution best;
        int bestCost = INT_MAX; //  +inf básicamente

        Solution curr;
        //  dfs( inst, idx, M, curr, currCost, best, bestCost )
        dfs( inst, 1, M, curr, /*currCost=*/0, best, bestCost );

        //  Si no encontramos ninguna solución factible, bestCost seguirá en INT_MAX y 'best' estará vacío.
        return best;
    }

}