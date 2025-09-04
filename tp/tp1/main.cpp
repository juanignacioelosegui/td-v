#include <iostream>
#include <string>
#include <vector>
#include <filesystem> 
#include <climits> 
#include <algorithm>
#include <chrono>
#include "Instance.h"
#include "Solution.h"
#include "BruteForceSolver.h"
#include "BacktrackingSolver.h"
using namespace std;

static bool run_one(const string& filepath, const string& algorithm) {
    Instance instance;
    if (!instance.loadFromFile(filepath)) {
        cerr << "[FAIL] No pude leer: " << filepath << "\n";
        return false;
    }
    cout << "\n===== " << filepath << " =====\n";
    cout << "=== Instancia cargada correctamente ===\n";

    if (algorithm == "fb" || algorithm == "both") {
        BruteForceSolver bruteForceSolver;

        auto t0_fb = std::chrono::steady_clock::now();
        Solution bruteForceSolution = bruteForceSolver.solve(instance, Solution(), 0);
        auto t1_fb = std::chrono::steady_clock::now();

        cout << "[FB] Solucion de Fuerza Bruta:\n";
        bruteForceSolution.printSolution();

        auto us_fb = std::chrono::duration_cast<std::chrono::microseconds>(t1_fb - t0_fb).count();
        cout << "[FB] Tiempo: " << us_fb << " microsec (" << (us_fb / 1000.0) << " ms)\n";
    }

    if (algorithm == "bt" || algorithm == "both") {
        BacktrackingSolver backtrackingSolver;

        auto t0_bt = std::chrono::steady_clock::now();
        Solution bestBT = Solution();
        bestBT.setCost(INT_MAX);
        Solution backtrackingSolution = backtrackingSolver.solve(instance, Solution(), 0, bestBT);
        auto t1_bt = std::chrono::steady_clock::now();

        cout << "[BT] Solucion de Backtracking:\n";
        backtrackingSolution.printSolution();

        auto us_bt = std::chrono::duration_cast<std::chrono::microseconds>(t1_bt - t0_bt).count();
        cout << "[BT] Tiempo: " << us_bt << " microsec (" << (us_bt / 1000.0) << " ms)\n";
    }

    cout << "---------------------------\n";
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Uso:\n"
             << "  " << argv[0] << " <instancia|ALL> <algoritmo>\n"
             << "  algoritmo: fb | bt | both\n";
        return 1;
    }

    string instanceArg = argv[1];
    string algorithm   = argv[2];

    // Caso 1: correr TODAS las instancias .txt de selected_instances/
    if (instanceArg == "ALL" || instanceArg == "all" || instanceArg == "*") {
        namespace fs = std::filesystem;
        fs::path dir = "selected_instances";
        if (!fs::exists(dir) || !fs::is_directory(dir)) {
            cerr << "No existe la carpeta: " << dir.string() << "\n";
            return 1;
        }
        vector<fs::path> files;
        for (const auto& e : fs::directory_iterator(dir)) {
            if (e.is_regular_file() && e.path().extension() == ".txt")
                files.push_back(e.path());
        }
        sort(files.begin(), files.end());
        if (files.empty()) {
            cerr << "No encontre .txt en " << dir.string() << "\n";
            return 1;
        }
        int ok = 0, fail = 0;
        for (const auto& f : files) (run_one(f.string(), algorithm) ? ++ok : ++fail);
        cout << "\nResumen: OK=" << ok << " FAIL=" << fail << "\n";
        return fail ? 2 : 0;
    }

    // Caso 2: correr UNA sola instancia (como ya hacías)
    string filename = "selected_instances/" + instanceArg + ".txt";
    bool ok = run_one(filename, algorithm);
    return ok ? 0 : 1;
}