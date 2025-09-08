#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <filesystem> 
#include <algorithm>
#include <chrono>
#include <climits>
#include <iomanip>
#include <sstream>
#include "Instance.h"
#include "Solution.h"
#include "BruteForceSolver.h"
#include "BacktrackingSolver.h"
using namespace std;

#include <fstream>

// Alias y helpers de tiempo
using clk = std::chrono::steady_clock;

static inline double ms_since(const clk::time_point& a, const clk::time_point& b) {
    return std::chrono::duration<double, std::milli>(b - a).count();
}

static int maxSegmentsPerInfluencer(const Instance& inst) {
    int max = 0;
    for (int i = 0; i < inst.getNumInfluencers(); i++) {
        const auto& inf = inst.getInfluencer(i);
        int ssz = (int)inf.second.size();
        if (ssz > max) max = ssz;
    }
    return max;
}

//¿Este subconjunto de influencers puede cubrir todos los N segmentos?
static bool canCoverAll(const Instance& inst) {
    std::set<int> covered;
    for (int i = 0; i < inst.getNumInfluencers(); ++i) {
        const auto& inf = inst.getInfluencer(i);
        const std::set<int>& segs = inf.second;   // sin copia
        covered.insert(segs.begin(), segs.end());
        if ((int)covered.size() == inst.getNumSegments()) return true;
    }
    return ((int)covered.size() == inst.getNumSegments());
}

//serializa la selección como "0 1 3 ..."
static std::string influencers_to_string(const Solution& sol) {
    std::string s;
    for (int i = 0; i < sol.InfluencerSize(); ++i) {
        if (i) s.push_back(' ');
        s += std::to_string(sol.getInfluencerPos(i));
    }
    return s;
}

static int maxSegmentId(const Instance& inst) {
    int mx = -1;
    for (int i = 0; i < inst.getNumInfluencers(); ++i) {
        const auto& segs = inst.getInfluencer(i).second;
        for (int s : segs) if (s > mx) mx = s;
    }
    return mx; // si no hay segmentos, queda -1
}


int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout << std::fixed << std::setprecision(3);

    // Leer combinaciones ya procesadas del CSV
    std::set<std::string> done;
    std::ifstream csv_in("resultados.csv");
    bool exists = csv_in.good();
    if (exists) {
        std::string line;
        getline(csv_in, line); // header
        while (getline(csv_in, line)) {
            std::istringstream iss(line);
            std::string instancia, k, algoritmo, tipo;
            getline(iss, instancia, ',');
            getline(iss, k, ',');
            getline(iss, algoritmo, ',');
            getline(iss, tipo, ',');
            done.insert(instancia + "," + k + "," + algoritmo + "," + tipo);
        }
        csv_in.close();
    }
    const std::string CSV_PATH = "resultados.csv";
    std::ofstream csv;

    if (exists) {
        csv.open(CSV_PATH, std::ios::app);
    } else {
        csv.open(CSV_PATH);
        csv << "Instancia,k,Algoritmo,Tipo,Costo_total,Tiempo_ms,Factible,Seleccion" << std::endl;
    }

    if (!csv.is_open()) {
        std::cerr << "No pude abrir " << CSV_PATH << " para escribir.\n";
        return 1;
    }

    std::cout << "Escribiendo resultados en: "
            << std::filesystem::absolute(CSV_PATH).string() << "\n";


    if (argc < 3) {
        cout << "Uso:\n"
             << "  " << argv[0] << " <instancia|ALL> <algoritmo>\n"
             << "  algoritmo: fb | bt | both\n";
        return 1;
    }

    string instanceArg = argv[1];
    string algorithm   = argv[2];

    auto write_csv = [&](const string& instancia, int k, const string& algoritmo,
                     const string& tipo, int costo, double tiempo_ms, bool factible,
                     const string& seleccion) {
    csv << instancia << "," << k << "," << algoritmo << "," << tipo << ","
        << costo << "," << tiempo_ms << "," << (factible ? "si" : "no") << ","
        << "\"" << seleccion << "\"" << std::endl;  // comillas por espacios
    csv.flush(); 
    };

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

        // Calcular total de combinaciones
        int total = 0;
        for (const auto& f : files) {
            Instance instance;
            if (!instance.loadFromFile(f.string())) continue;
            int maxK = maxSegmentsPerInfluencer(instance);
            if (maxK <= 0) continue;
            int kStart = std::min(5, maxK);
            int numK = ((maxK - kStart) / 5) + 1;
            int algs = (algorithm == "both") ? 2 : 1;
            total += numK * algs * 2; // 2 tipos: MICRO y MACRO
        }

        int current = 0;
        for (const auto& f : files) {
            string instancia = f.filename().string();
            Instance instance;
            if (!instance.loadFromFile(f.string())) {
                ++fail;
                continue;
            }
            int maxK = maxSegmentsPerInfluencer(instance);
            if (maxK <= 0) {
                ++fail;
                continue;
            }
            int kStart = std::min(5, maxK);
            for (int k = kStart; k <= maxK; k += 5) {
                auto parts = instance.splitBySegmentCount(k);
                const Instance& microInst = parts.first;
                const Instance& macroInst = parts.second;

                // MICRO
                bool microFactible = canCoverAll(microInst);
                if (algorithm == "fb" || algorithm == "both") {
                    std::string key = instancia + "," + std::to_string(k) + ",fb,MICRO";
                    if (!done.count(key)) {
                        int costo = -1;
                        double tiempo = 0.0;
                        std::string sel;
                        if (microFactible) {
                            BruteForceSolver bruteForceSolver;
                            Solution sfb; 
                            sfb.initCover(maxSegmentId(microInst) + 1);
                            auto t0 = clk::now();
                            Solution sol = bruteForceSolver.solve(microInst, sfb, 0);
                            auto t1 = clk::now();
                            costo = sol.getCost();
                            tiempo = ms_since(t0, t1);
                            sel = influencers_to_string(sol);
                            cout << "  [MICRO][FB] " << tiempo << " ms, costo=" << costo
                            << ", sel=[" << sel << "]\n"; 
                        }
                        write_csv(instancia, k, "fb", "MICRO", costo, tiempo, microFactible, sel);
                    }
                    ++current;
                    int barWidth = 40;
                    double progress = (double)current / total;
                    int pos = (int)(barWidth * progress);
                    cout << "Progreso [";
                    for (int i = 0; i < barWidth; ++i) cout << (i < pos ? '#' : ' ');
                    cout << "] " << int(progress * 100) << "% " << current << "/" << total << "\r";
                    cout.flush();
                }
                if (algorithm == "bt" || algorithm == "both") {
                    std::string key = instancia + "," + std::to_string(k) + ",bt,MICRO";
                    if (!done.count(key)) {
                        int costo = -1;
                        double tiempo = 0.0;
                        std::string sel;
                        if (microFactible) {
                            BacktrackingSolver backtrackingSolver;
                            Solution parcial; 
                            parcial.initCover(microInst.getNumSegments() + 1);
                            Solution best; best.setCost(INT_MAX);
                            auto t0 = clk::now();
                            Solution sol = backtrackingSolver.solve(microInst, parcial, 0, best);
                            auto t1 = clk::now();
                            costo = sol.getCost();
                            tiempo = ms_since(t0, t1);
                            sel = influencers_to_string(sol);
                            cout << "  [MICRO][BT] " << tiempo << " ms, costo=" << costo
                            << ", sel=[" << sel << "]\n";
                        }
                        write_csv(instancia, k, "bt", "MICRO", costo, tiempo, microFactible, sel);
                    }
                    ++current;
                    int barWidth = 40;
                    double progress = (double)current / total;
                    int pos = (int)(barWidth * progress);
                    cout << "Progreso [";
                    for (int i = 0; i < barWidth; ++i) cout << (i < pos ? '#' : ' ');
                    cout << "] " << int(progress * 100) << "% " << current << "/" << total << "\r";
                    cout.flush();
                }

                // MACRO
                bool macroFactible = canCoverAll(macroInst);
                if (algorithm == "fb" || algorithm == "both") {
                    std::string key = instancia + "," + std::to_string(k) + ",fb,MACRO";
                    if (!done.count(key)) {
                        int costo = -1;
                        double tiempo = 0.0;
                        std::string sel;
                        if (macroFactible) {
                            BruteForceSolver bruteForceSolver;
                            Solution sfb; 
                            sfb.initCover(macroInst.getNumSegments() + 1);
                            auto t0 = clk::now();
                            Solution sol = bruteForceSolver.solve(macroInst, sfb, 0);
                            auto t1 = clk::now();
                            costo = sol.getCost();
                            tiempo = ms_since(t0, t1);
                            sel = influencers_to_string(sol);
                            cout << "  [MACRO][FB] " << tiempo << " ms, costo=" << costo
                            << ", sel=[" << sel << "]\n";  
                        }
                        write_csv(instancia, k, "fb", "MACRO", costo, tiempo, macroFactible, sel);
                    }
                    ++current;
                    int barWidth = 40;
                    double progress = (double)current / total;
                    int pos = (int)(barWidth * progress);
                    cout << "Progreso [";
                    for (int i = 0; i < barWidth; ++i) cout << (i < pos ? '#' : ' ');
                    cout << "] " << int(progress * 100) << "% " << current << "/" << total << "\r";
                    cout.flush();
                }
                if (algorithm == "bt" || algorithm == "both") {
                    std::string key = instancia + "," + std::to_string(k) + ",bt,MACRO";
                    if (!done.count(key)) {
                        int costo = -1;
                        double tiempo = 0.0;
                        std::string sel;
                        if (macroFactible) {
                            BacktrackingSolver backtrackingSolver;
                            Solution parcial; 
                            parcial.initCover(macroInst.getNumSegments() + 1);
                            Solution best; best.setCost(INT_MAX);
                            auto t0 = clk::now();
                            Solution sol = backtrackingSolver.solve(macroInst, parcial, 0, best);
                            auto t1 = clk::now();
                            costo = sol.getCost();
                            tiempo = ms_since(t0, t1);
                            sel = influencers_to_string(sol);
                            cout << "  [MACRO][BT] " << tiempo << " ms, costo=" << costo
                            << ", sel=[" << sel << "]\n";   
                        }
                        write_csv(instancia, k, "bt", "MACRO", costo, tiempo, macroFactible, sel);
                    }
                    ++current;
                    int barWidth = 40;
                    double progress = (double)current / total;
                    int pos = (int)(barWidth * progress);
                    cout << "Progreso [";
                    for (int i = 0; i < barWidth; ++i) cout << (i < pos ? '#' : ' ');
                    cout << "] " << int(progress * 100) << "% " << current << "/" << total << "\r";
                    cout.flush();
                }
            }
            ++ok;
        }
        cout << endl << "\nResumen: OK=" << ok << " FAIL=" << fail << "\n";
        return fail ? 2 : 0;
    }

    // Caso 2: correr UNA sola instancia
    string filename = "selected_instances/" + instanceArg + ".txt";
    string instancia = instanceArg + ".txt";
    Instance instance;
    if (!instance.loadFromFile(filename)) {
        cerr << "No pude leer: " << filename << endl;
        return 1;
    }
    int maxK = maxSegmentsPerInfluencer(instance);
    if (maxK <= 0) {
        cerr << "Instancia sin cobertura." << endl;
        return 1;
    }
    int kStart = std::min(5, maxK);
    for (int k = kStart; k <= maxK; k += 5) {
        auto parts = instance.splitBySegmentCount(k);
        const Instance& microInst = parts.first;
        const Instance& macroInst = parts.second;

        // MICRO
        bool microFactible = canCoverAll(microInst);
        if (algorithm == "fb" || algorithm == "both") {
            int costo = -1;
            double tiempo = 0.0;
            std::string sel;
            if (microFactible) {
                BruteForceSolver bruteForceSolver;
                Solution sfb;
                sfb.initCover(microInst.getNumSegments() + 1);
                auto t0 = clk::now();
                Solution sol = bruteForceSolver.solve(microInst, sfb, 0);
                auto t1 = clk::now();
                costo = sol.getCost();
                tiempo = ms_since(t0, t1);
                sel = influencers_to_string(sol);
            }
            write_csv(instancia, k, "fb", "MICRO", costo, tiempo, microFactible, sel);
        }
        if (algorithm == "bt" || algorithm == "both") {
            int costo = -1;
            double tiempo = 0.0;
            std::string sel;
            if (microFactible) {
                BacktrackingSolver backtrackingSolver;
                Solution parcial; 
                parcial.initCover(microInst.getNumSegments() + 1);
                Solution best; best.setCost(INT_MAX);
                auto t0 = clk::now();
                Solution sol = backtrackingSolver.solve(microInst, parcial, 0, best);
                auto t1 = clk::now();
                costo = sol.getCost();
                tiempo = ms_since(t0, t1);
                sel = influencers_to_string(sol);
            }
            write_csv(instancia, k, "bt", "MICRO", costo, tiempo, microFactible, sel);
        }

        // MACRO
        bool macroFactible = canCoverAll(macroInst);
        if (algorithm == "fb" || algorithm == "both") {
            int costo = -1;
            double tiempo = 0.0;
            std::string sel;
            if (macroFactible) {
                BruteForceSolver bruteForceSolver;
                Solution sfb; 
                sfb.initCover(macroInst.getNumSegments() + 1);
                auto t0 = clk::now();
                Solution sol = bruteForceSolver.solve(macroInst, sfb, 0);
                auto t1 = clk::now();
                costo = sol.getCost();
                tiempo = ms_since(t0, t1);
                sel = influencers_to_string(sol);
            }
            write_csv(instancia, k, "fb", "MACRO", costo, tiempo, macroFactible, sel);
        }
        if (algorithm == "bt" || algorithm == "both") {
            int costo = -1;
            double tiempo = 0.0;
            std::string sel;
            if (macroFactible) {
                BacktrackingSolver backtrackingSolver;
                Solution parcial; 
                parcial.initCover(macroInst.getNumSegments() + 1);
                Solution best; best.setCost(INT_MAX);
                auto t0 = clk::now();
                Solution sol = backtrackingSolver.solve(macroInst, parcial, 0, best);
                auto t1 = clk::now();
                costo = sol.getCost();
                tiempo = ms_since(t0, t1);
                sel = influencers_to_string(sol);
            }
            write_csv(instancia, k, "bt", "MACRO", costo, tiempo, macroFactible, sel);
        }
    }
    return 0;
}