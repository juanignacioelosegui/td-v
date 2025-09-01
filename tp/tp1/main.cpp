#include <iostream>
#include <string>
#include <vector>
#include "Instance.h"
#include "Solution.h"
#include "BruteForceSolver.h"

using namespace std;

// ==== PONÉ TU RUTA ACÁ ====
// Carpeta con todas las instancias:
static const string INST_DIR  = "/home/chipi/Desktop/td-v/tp/tp1/selected_instances/";
// Archivo por defecto a usar dentro de esa carpeta:
static const string INST_FILE = "ninstance_305.txt";
// ==========================

int main(int argc, char* argv[])
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1) Resolver la ruta de la instancia
    string instancePath;
    if ( argc >= 2 )
    {
        // Si pasás la ruta completa por CLI, se usa esa
        // Ej: ./prog /ruta/a/instancia.txt
        instancePath = argv[1];
        std::cout << "Usando ruta recibida por CLI: " << instancePath << "\n";
    } else {
        //  Si no, usa INST_DIR + INST_FILE (modificá arriba)
        instancePath = INST_DIR + INST_FILE;
        std::cout << "Usando ruta por defecto: " << instancePath << "\n";
        std::cout << "Tip: podés pasar ruta completa por CLI: ./prog /ruta/a/instancia.txt\n";
    }

    // 2) Cargar la instancia elegida
    Instance inst;
    std::cout << "Cargando archivo '" << instancePath << "'...\n";
    if ( !inst.loadFromFile( instancePath ) ) {
        cerr << "Error al leer la instancia: " << instancePath << "\n";
        return 1;
    }
    std::cout << "Instancia cargada. N=" << inst.getNumSegments()
         << " M=" << inst.getNumInfluencers() << "\n\n";

    // 3) CASO A: Operaciones básicas de Solution
    std::cout << "=== CASO A: add/remove y printSolution ===\n";
    Solution sol;

    std::cout << "1) addInfluencer(6)\n";
    std::cout << "[Esperado] Si M < 6 -> mensaje 'no es válido'. Si M >= 6 y existe 6 -> lo agrega.\n";
    sol.addInfluencer( 6, inst );

    std::cout << "2) addInfluencer(1)\n";
    std::cout << "[Esperado] Si existe 1 -> lo agrega (sin error).\n";
    sol.addInfluencer( 1, inst );

    std::cout << "3) addInfluencer(3)\n";
    std::cout << "[Esperado] Si existe 3 -> lo agrega (sin error).\n";
    sol.addInfluencer( 3, inst );

    std::cout << "4) addInfluencer(1) de nuevo\n";
    std::cout << "[Esperado] Mensaje: 'El influencer 1 ya está en la solución.'\n";
    sol.addInfluencer( 1, inst );

    std::cout << "\nprintSolution()\n";
    std::cout << "[Esperado] Lista de influencers elegidos, costo total acumulado y segmentos cubiertos.\n\n";
    sol.printSolution();

    std::cout << "5) removeInfluencer(3)\n";
    std::cout << "[Esperado] Si 3 estaba, se elimina; si no, mensaje 'no está en la solución'.\n";
    sol.removeInfluencer( 3 );

    std::cout << "6) removeInfluencer(4)\n";
    std::cout << "[Esperado] Si 4 no estaba, mensaje 'no está en la solución'.\n";
    sol.removeInfluencer( 4 );

    std::cout << "\nprintSolution()\n";
    std::cout << "[Esperado] Refleja el estado actualizado tras las eliminaciones.\n\n";
    sol.printSolution();

    // 4) CASO B: Cobertura y helpers
    std::cout << "=== CASO B: coversAll / uncovered / totalCost ===\n";
    std::cout << "Con el subconjunto actual:\n";
    std::cout << "coversAll: " << ( sol.coversAll( inst ) ? "true" : "false" ) << "\n";
    auto falta = sol.uncoveredSegments( inst );
    std::cout << "uncovered: {";
    bool first = true;
    for ( int s : falta ) { if ( !first ) std::cout << ","; std::cout << s; first = false; }
    std::cout << "}\n";
    std::cout << "totalCost: " << sol.totalCost() << "\n\n";

    // 5) CASO C: Fuerza bruta (óptimo sobre esta instancia real)
    std::cout << "=== CASO C: Fuerza Bruta sobre la instancia elegida ===\n";
    std::cout << "[Nota] Imprime la solución óptima encontrada (conjunto, costo y cobertura).\n";
    {
        Solution best = BruteForceSolver::solve( inst );
        best.printSolution();
        std::cout << "coversAll: " << ( best.coversAll( inst ) ? "true" : "false" ) << "\n";
        std::cout << "totalCost: " << best.totalCost() << "\n";
    }

    std::cout << "\n=== Fin de pruebas ===\n";
    return 0;
}