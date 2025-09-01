# Influencer Marketing Optimization Problem
## Estructura del Proyecto

### Clases Principales

1. **Instance** (`Instance.h`, `Instance.cpp`)
   - Carga y almacena las instancias del problema
   - Proporciona acceso a los datos del problema
  
2. **Solution** (`Solution.h`, `Solution.cpp`)
   - Representa una solución

3. **BruteForceSolver** (`BruteForceSolver.h`, `BruteForceSolver.cpp`)
   - Resuelve utilizando fuerza bruta

4. **BacktrackingSolver** (`BacktrackingSolver.h`, `BacktrackingSolver.cpp`)
   - Resuelve utilizando backtracking

### Otros archivos

- **main.cpp**: Demuestra un poco el uso de todas las clases. Customizar todo lo que sea necesario.
- **Makefile**: Para compilación fácil.
- **selected_instances/**: Carpeta con archivos de instancias.

## Instrucciones para Ejecutar

### Prerequisitos
- Compilador C++ con soporte para C++17 (g++, clang++, etc.)
- Make (opcional, pero recomendado)

### Compilar el proyecto:
```bash
make
```
Hay otras opciones de compilación en el Makefile, son libres de investigarlas y usarlas.

### Compilación manual (sin Make):
```bash
g++ -Wall -Wextra -std=c++17 -O2 -o influencer_solver main.cpp Instance.cpp Solution.cpp BruteForceSolver.cpp BacktrackingSolver.cpp
```

### Ejecutar con una instancia específica y algoritmo:
```bash
./influencer_solver selected_instances/ninstance_305.txt fb
```
