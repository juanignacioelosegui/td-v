
#   influencer_cover.py
#   Parser más sofisticado, soporta comas o espacios en las instancias

#   Este archivo es una 'traducción' de los códigos en C++.
#   Están todos los archivos .cpp y .h consolidados en este único .py.
from __future__ import annotations

from dataclasses import dataclass, field
from typing import List, Set, Tuple, Optional
import time
import argparse

#   Reemplazar comas por espacios
def _split_tokens(s: str):
    return [t for t in s.replace(',', ' ').split() if t]

@dataclass
class Instance:
    N: int = 0
    M: int = 0
    influencers: List[Tuple[int, Set[int]]] = field(default_factory=list)

    def load_from_file(self, filename: str) -> bool:
        lines: List[str] = []
        try:
            with open(filename, "r", encoding="utf-8", errors="replace") as f:
                for raw in f:
                    s = raw.strip()
                    if not s or s.startswith("#") or s.startswith("//"):
                        continue
                    lines.append(s)
        except OSError as e:
            print(f"Error: Cannot open file {filename}: {e}")
            return False

        if not lines:
            print(f"Error: File {filename} is empty.")
            return False

        def try_parse_two_ints(s: str):
            parts = _split_tokens(s)
            if len(parts) == 2:
                try:
                    return int(parts[0]), int(parts[1])
                except ValueError:
                    return None
            return None

        header = try_parse_two_ints(lines[0])
        self.influencers.clear()

        if header is not None:
            self.N, self.M = header
            body = lines[1:]
            if len(body) < self.M:
                print(f"Warning: expected {self.M} influencer lines, got {len(body)}. Using available lines.")
            for idx, ln in enumerate(body[:self.M]):
                parts = _split_tokens(ln)
                if not parts:
                    continue
                try:
                    cost = int(parts[0])
                    segs = {int(x) for x in parts[1:]}
                except ValueError:
                    print(f"Warning: could not parse line {idx+2}: {ln!r}")
                    continue
                self.influencers.append((cost, segs))
        else:
            max_seg = -1
            any_zero = False
            for ln in lines:
                parts = _split_tokens(ln)
                if not parts:
                    continue
                try:
                    cost = int(parts[0])
                    segs_list = [int(x) for x in parts[1:]]
                except ValueError:
                    print(f"Warning: could not parse line: {ln!r}")
                    continue
                for s in segs_list:
                    if s == 0: any_zero = True
                    if s > max_seg: max_seg = s
                self.influencers.append((cost, set(segs_list)))
            self.M = len(self.influencers)
            if max_seg >= 0:
                self.N = max_seg + 1 if any_zero else max_seg
            else:
                self.N = 0

        if self.M != len(self.influencers):
            self.M = len(self.influencers)

        if self.N <= 0:
            all_segs = set().union(*(segs for _, segs in self.influencers)) if self.influencers else set()
            if all_segs:
                any_zero = (0 in all_segs)
                self.N = (max(all_segs) + 1) if any_zero else max(all_segs)

        return True

    def get_num_segments(self) -> int:
        return self.N

    def get_num_influencers(self) -> int:
        return self.M

    def get_influencer(self, i: int) -> Tuple[int, Set[int]]:
        return self.influencers[i]

@dataclass
class Solution:
    _costo_total: int = 0
    _influencers_usados: List[int] = field(default_factory=list)
    def add_influencer(self, influencer: int, costo: int) -> None:
        self._influencers_usados.append(influencer)
        self._costo_total += costo
    def remove_influencer(self, influencer: int, costo: int) -> None:
        if self._influencers_usados and self._influencers_usados[-1] == influencer:
            self._influencers_usados.pop()
        else:
            try: self._influencers_usados.remove(influencer)
            except ValueError: pass
        self._costo_total -= costo
    def set_cost(self, c: int): self._costo_total = c
    def get_cost(self) -> int: return self._costo_total
    def get_influencer_pos(self, pos: int) -> int: return self._influencers_usados[pos]
    def influencer_size(self) -> int: return len(self._influencers_usados)
    def copy(self) -> "Solution": return Solution(self._costo_total, list(self._influencers_usados))

#   Funciones auxiliares
def _covered_segments(sol: Solution, instance: Instance) -> Set[int]:
    segs: Set[int] = set()
    for i in range(sol.influencer_size()):
        idx = sol.get_influencer_pos(i)
        segs |= instance.get_influencer(idx)[1]
    return segs

def _covers_all(sol: Solution, instance: Instance) -> bool:
    return len(_covered_segments(sol, instance)) == instance.get_num_segments()

def _min_cost(a: Optional[Solution], b: Optional[Solution]) -> Optional[Solution]:
    if a is None: return b
    if b is None: return a
    return a if a.get_cost() <= b.get_cost() else b

def _is_better_or_equal(a: Optional[Solution], b: Optional[Solution]) -> bool:
    if b is None: return True
    if a is None: return False
    return a.get_cost() <= b.get_cost()

class BruteForceSolver:
    def solve(self, instance: Instance, sol_parcial: Solution, i: int) -> Optional[Solution]:
        M = instance.get_num_influencers()
        if i == M:
            return sol_parcial.copy() if _covers_all(sol_parcial, instance) else None
        best_without = self.solve(instance, sol_parcial, i + 1)
        costo_i, _ = instance.get_influencer(i)
        sol_parcial.add_influencer(i, costo_i)
        best_with = self.solve(instance, sol_parcial, i + 1)
        sol_parcial.remove_influencer(i, costo_i)
        return _min_cost(best_with, best_without)

class BacktrackingSolver:
    def solve(self, instance: Instance, sol_parcial: Solution, i: int, best_solution: Optional[Solution]) -> Optional[Solution]:
        if best_solution is not None and sol_parcial.get_cost() >= best_solution.get_cost():
            return best_solution
        M = instance.get_num_influencers()
        if i == M:
            return _min_cost(sol_parcial.copy() if _covers_all(sol_parcial, instance) else None, best_solution)
        if _covers_all(sol_parcial, instance):
            return _min_cost(sol_parcial.copy(), best_solution)
        sol_b = self.solve(instance, sol_parcial, i + 1, best_solution)
        improved_best = sol_b if _is_better_or_equal(sol_b, best_solution) else best_solution
        costo_i, _ = instance.get_influencer(i)
        sol_parcial.add_influencer(i, costo_i)
        sol_a = self.solve(instance, sol_parcial, i + 1, improved_best)
        sol_parcial.remove_influencer(i, costo_i)
        return _min_cost(sol_a, sol_b)

def run_one(filepath: str, algorithm: str) -> bool:
    instance = Instance()
    if not instance.load_from_file(filepath):
        print(f"[FAIL] No pude leer: {filepath}")
        return False
    start = time.perf_counter()
    if algorithm.lower() in ("bf","bruteforce","brute"):
        solver = BruteForceSolver()
        best = solver.solve(instance, Solution(), 0)
    elif algorithm.lower() in ("bt", "backtracking"):
        solver = BacktrackingSolver()
        best = solver.solve(instance, Solution(), 0, None)
    else:
        print(f"[FAIL] Algoritmo desconocido: {algorithm}. Usá 'bf' o 'bt'.")
        return False
    elapsed = time.perf_counter() - start
    if best is None:
        print(f"[FAIL] No se encontró solución factible.  ({elapsed:.3f}s)")
        return False
    else:
        print(f"[OK]  Costo óptimo = {best.get_cost()}  |  Influencers usados = {best._influencers_usados}  ({elapsed:.3f}s)")
        return True

def main(argv=None) -> int:
    p = argparse.ArgumentParser(description="Influencer coverage solver (Python port, robust CSV parser)")
    p.add_argument("--dir", type=str, default=None)
    p.add_argument("--instance", type=str, default=None)
    p.add_argument("--file", type=str, default=None)
    p.add_argument("--algo", type=str, default="bt", choices=["bt","bf","backtracking","bruteforce","brute"])
    args = p.parse_args(argv)
    if args.file is not None:
        return 0 if run_one(args.file, args.algo) else 1
    if args.dir is not None:
        import pathlib
        dirp = pathlib.Path(args.dir)
        files = sorted([p for p in dirp.glob("*.txt")])
        ok = fail = 0
        for f in files:
            print(f"\n=== {f} ===")
            (run_one(str(f), args.algo) and (ok:=ok+1)) or (fail:=fail+1)
        print(f"\nResumen: OK={ok} FAIL={fail}")
        return 0 if fail == 0 else 2
    if args.instance is not None:
        filename = f"selected_instances/{args.instance}.txt"
        return 0 if run_one(filename, args.algo) else 1
    p.print_help()
    return 1

#   Main
if __name__ == "__main__":
    raise SystemExit(main())