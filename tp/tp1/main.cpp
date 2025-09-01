#include <iostream>
#include <string>
#include "Instance.h"
#include "Solution.h"
//#include "BruteForceSolver.h"
//#include "BacktrackingSolver.h"
#include <vector>

int main() {
// 1. Crear instancia y cargar manualmente influencers
    Instance inst = {};
    inst.loadFromFile("C:/Users/juani/OneDrive/Documentos/Di Tella/2025/TD V/templatetp1/template/selected_instances/prueba.txt");
    Solution sol(3);
    std::cout << "Cargando archivo..." << std::endl;

    // 3. Probar agregar influencers válidos
    sol.addInfluencer(6, inst); // no debería insertarlo
    sol.addInfluencer(1, inst); // debería insertarlo
    sol.addInfluencer(3, inst);
    sol.addInfluencer(1, inst); // debería mostrar "Ya está en el diccionario ó no es válido"

    // 4. Probar agregar un influencer inválido (no está en Instance)
    sol.addInfluencer(5, inst); // debería mostrar "Ya está en el diccionario ó no es válido"

    sol.printSolution();

    // 5. Probar eliminar influencers
    sol.removeInfluencer(3); // debería eliminarlo
    sol.removeInfluencer(4); // debería mostrar "No está en el diccionario"
    
    sol.printSolution();
    return 0;
}

/*
int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cout << "Uso: " << argv[0] << " <instancia> <algoritmo>" << std::endl;
        std::cout << "Algoritmo: fb para Fuerza Bruta, bt para Backtracking" << std::endl;
        return 1;
    }

    std::string filename = "selected_instances/" + std::string(argv[1]) + ".txt";
    // Load instance
    Instance instance;
    if (!instance.loadFromFile(filename)) {
        std::cerr << "Error al leer la instancia: " << filename << std::endl;
        return 1;
    }
    std::cout << "=== Instancia cargada correctamente ===" << std::endl;

    std::string algorithm = argv[2];
    if (algorithm == "fb") {
        BruteForceSolver bruteForceSolver;
        Solution bruteForceSolution = bruteForceSolver.solve(instance);
        std::cout << "Solucion de Fuerza Bruta:" << std::endl;
        bruteForceSolution.printSolution();
    }
    
    if (algorithm == "bt") {
        BacktrackingSolver backtrackingSolver;
        Solution backtrackingSolution = backtrackingSolver.solve(instance);
        std::cout << "Solucion de Backtracking:" << std::endl;
        backtrackingSolution.printSolution();
    }
    
    return 0;
}
*/