# -*- coding: utf-8 -*-
"""
Influencer Cover (Python) – exact solvers + CSV recomputation

Input format (per enunciado):
- 1st line: N (segments, 1..N)
- 2nd line: M (influencers)
- Next M lines: "cost, seg1, seg2, ..." with seg in 1..N

Implements:
- Instance / Solution
- BruteForceSolver / BacktrackingSolver
- CLI for single instance, plus CSV mode: --csv-in/--csv-out [--instances-dir]
"""
import sys
import argparse
from dataclasses import dataclass, field
from typing import List, Set, Tuple, Optional

INF = 10**18

# --------------------------
# Instance
# --------------------------
@dataclass
class Instance:
    N: int = 0
    M: int = 0
    _instancia: List[Tuple[int, Set[int]]] = field(default_factory=list)

    def getNumSegments(self) -> int:
        return self.N

    def getNumInfluencers(self) -> int:
        return self.M

    def getInfluencer(self, i: int) -> Tuple[int, Set[int]]:
        return self._instancia[i]

    def printInstance(self) -> None:
        print(f"Instance: N={self.N}, M={self.M}")
        for i, (c, segs) in enumerate(self._instancia):
            print(f"  [{i}] cost={c} segments={sorted(segs)}")

    def splitBySegmentCount(self, k: int) -> Tuple["Instance", "Instance"]:
        micro = Instance(N=self.N)
        macro = Instance(N=self.N)
        for cost, segs in self._instancia:
            if len(segs) < k:
                micro._instancia.append((cost, set(segs)))
            else:
                macro._instancia.append((cost, set(segs)))
        micro.M = len(micro._instancia)
        macro.M = len(macro._instancia)
        return micro, macro

    @staticmethod
    def _parse_csv_line(line: str) -> Optional[Tuple[int, Set[int]]]:
        if not line.strip():
            return None
        parts = [p.strip() for p in line.split(",") if p.strip() != ""]
        if not parts:
            return None
        try:
            cost = int(parts[0])
        except ValueError:
            return None
        segs: Set[int] = set()
        for token in parts[1:]:
            try:
                seg = int(token)
            except ValueError:
                continue
            segs.add(seg)
        return cost, segs

    def loadFromFile(self, filename: str) -> bool:
        try:
            with open(filename, "r", encoding="utf-8") as f:
                lines = [ln.strip() for ln in f if ln.strip() != ""]
        except OSError:
            print(f"Error: cannot open file '{filename}'", file=sys.stderr)
            return False

        if len(lines) < 2:
            print("Error: file must have at least two lines (N and M).", file=sys.stderr)
            return False

        try:
            N = int(lines[0].split()[0])
            M = int(lines[1].split()[0])
        except ValueError:
            print("Error: first two lines must be integers: N then M.", file=sys.stderr)
            return False

        data_lines = lines[2:]
        if len(data_lines) < M:
            print(f"Error: expected {M} influencer lines, found {len(data_lines)}.", file=sys.stderr)
            return False

        parsed: List[Tuple[int, Set[int]]] = []
        for i in range(M):
            item = Instance._parse_csv_line(data_lines[i])
            if item is None:
                print(f"Warning: skipping malformed line {i+3}: {data_lines[i]}", file=sys.stderr)
                parsed.append((0, set()))
                continue
            parsed.append(item)

        # Validate segments are 1..N
        for idx, (_c, segs) in enumerate(parsed):
            for s in segs:
                if s < 1 or s > N:
                    print(f"Warning: influencer {idx} has segment id {s} outside 1..{N}.", file=sys.stderr)
        self.N = N
        self.M = M
        self._instancia = [(int(c), set(map(int, s))) for c, s in parsed]
        return True

# --------------------------
# Solution
# --------------------------
@dataclass
class Solution:
    _costoTotal: int = 0
    _influencersUsados: List[int] = field(default_factory=list)
    _segmentosCount: dict = field(default_factory=dict)
    _coveredCount: int = 0

    def clear(self) -> None:
        self._costoTotal = 0
        self._influencersUsados.clear()
        self._segmentosCount.clear()
        self._coveredCount = 0

    def addInfluencer(self, idx: int, costo: int, segs: Set[int]) -> None:
        self._influencersUsados.append(idx)
        self._costoTotal += costo
        for s in segs:
            prev = self._segmentosCount.get(s, 0)
            if prev == 0:
                self._coveredCount += 1
            self._segmentosCount[s] = prev + 1

    def removeInfluencer(self, idx: int, costo: int, segs: Set[int]) -> None:
        if self._influencersUsados and self._influencersUsados[-1] == idx:
            self._influencersUsados.pop()
        elif idx in self._influencersUsados:
            self._influencersUsados.remove(idx)
        self._costoTotal -= costo
        for s in segs:
            prev = self._segmentosCount.get(s, 0)
            if prev == 1:
                self._coveredCount -= 1
                del self._segmentosCount[s]
            elif prev > 1:
                self._segmentosCount[s] = prev - 1

    def getCost(self) -> int:
        return self._costoTotal

    def getCoveredCount(self) -> int:
        return self._coveredCount

    def getUsedInfluencers(self) -> List[int]:
        return list(self._influencersUsados)

    def setCost(self, c: int) -> None:
        self._costoTotal = c

    def copy(self) -> "Solution":
        return Solution(self._costoTotal, list(self._influencersUsados),
                        dict(self._segmentosCount), self._coveredCount)

    def __lt__(self, other: "Solution") -> bool:
        return self._costoTotal < other._costoTotal

# --------------------------
# Solvers
# --------------------------
class BruteForceSolver:
    def solve(self, inst: Instance, sol: Solution, i: int) -> Solution:
        if i == inst.getNumInfluencers():
            if sol.getCoveredCount() == inst.getNumSegments():
                return sol.copy()
            bad = Solution(); bad.setCost(INF); return bad
        best_excl = self.solve(inst, sol, i+1)
        cost, segs = inst.getInfluencer(i)
        sol.addInfluencer(i, cost, segs)
        best_incl = self.solve(inst, sol, i+1)
        sol.removeInfluencer(i, cost, segs)
        return best_incl if best_incl.getCost() < best_excl.getCost() else best_excl

class BacktrackingSolver:
    def solve(self, inst: Instance, sol: Solution, i: int, best: Solution) -> Solution:
        best_cost = best.getCost()
        if sol.getCost() >= best_cost:
            bad = Solution(); bad.setCost(INF); return bad
        if i == inst.getNumInfluencers():
            if sol.getCoveredCount() == inst.getNumSegments():
                return sol.copy()
            bad = Solution(); bad.setCost(INF); return bad
        cost, segs = inst.getInfluencer(i)
        sol.addInfluencer(i, cost, segs)
        cand_incl = self.solve(inst, sol, i+1, best)
        if cand_incl.getCost() < best_cost:
            best = cand_incl; best_cost = best.getCost()
        sol.removeInfluencer(i, cost, segs)
        cand_excl = self.solve(inst, sol, i+1, best)
        if cand_excl.getCost() < best_cost:
            best = cand_excl
        return best

# --------------------------
# Variant helper
# --------------------------
def filter_variant(inst: Instance, k: Optional[int], mode: Optional[str]) -> Instance:
    if mode is None or k is None:
        return inst
    micro, macro = inst.splitBySegmentCount(k)
    if mode == "micro":
        return micro
    if mode == "macro":
        return macro
    return inst

# --------------------------
# CLI helpers
# --------------------------
def audit_instance(inst: Instance):
    union = set()
    for _, segs in inst._instancia:
        union |= segs
    missing = set(range(1, inst.N + 1)) - union
    return (len(missing) == 0), missing

# --------------------------
# CSV recomputation helpers
# --------------------------
def _normalize_algo(v: str) -> str:
    v = (v or "").strip().lower()
    if v == "fb":
        return "bf"
    if v in ("bf","bt"):
        return v
    return "bt"

def _normalize_tipo(v: str) -> str:
    v = (v or "").strip().upper()
    if v in ("MICRO","MACRO"):
        return v
    return "ALL"

def solve_instance_for_csv(file_path: str, algo: str, tipo: str, k: Optional[int]):
    inst = Instance()
    if not inst.loadFromFile(file_path):
        return "no", -1, []
    if tipo == "MICRO":
        inst = filter_variant(inst, k, "micro")
    elif tipo == "MACRO":
        inst = filter_variant(inst, k, "macro")
    if algo == "bt":
        solver = BacktrackingSolver()
        best = Solution(); best.setCost(INF)
        sol = solver.solve(inst, Solution(), 0, best)
    else:
        solver = BruteForceSolver()
        sol = solver.solve(inst, Solution(), 0)
    feasible = (sol.getCost() < INF) and (sol.getCoveredCount() == inst.getNumSegments())
    return ("si" if feasible else "no"), (sol.getCost() if feasible else -1), (sol.getUsedInfluencers() if feasible else [])

def recompute_csv(csv_in: str, csv_out: str, instances_dir: Optional[str] = None) -> None:
    import pandas as pd, json, os
    df = pd.read_csv(csv_in, engine="python")
    feas, costs, sels = [], [], []
    for _, row in df.iterrows():
        inst_name = str(row.get("Instancia","")).strip()
        if not inst_name:
            feas.append("no"); costs.append(-1); sels.append("[]"); continue
        file_path = os.path.join(instances_dir, inst_name) if instances_dir else inst_name
        algo = _normalize_algo(row.get("Algoritmo","bt"))
        tipo = _normalize_tipo(row.get("Tipo","ALL"))
        k = row.get("k", None)
        try:
            if k is None or (isinstance(k, float) and (k != k)):
                k_int = None
            else:
                k_int = int(k)
        except Exception:
            k_int = None
        f, c, s = solve_instance_for_csv(file_path, algo, tipo, k_int)
        feas.append(f)
        costs.append(c)
        try:
            sels.append(json.dumps(list(map(int, s))))
        except Exception:
            sels.append("[]")
    df["Factible"] = feas
    df["Costo_total"] = costs
    df["Seleccion"] = sels
    df.to_csv(csv_out, index=False)

# --------------------------
# Single-instance run
# --------------------------
def run(args: argparse.Namespace) -> int:
    base = Instance()
    if not base.loadFromFile(args.file):
        return 1
    inst = filter_variant(base, args.k, args.variant)
    if args.verbose:
        print("=== Loaded Instance ===")
        inst.printInstance()
    ok_by_union, missing = audit_instance(inst)
    if not ok_by_union and args.variant == "all":
        if args.audit:
            print("\n=== PRECHECK ===")
            print("Union of all influencers does NOT cover all segments.")
            ms = sorted(missing); sample = ms[:50]
            print(f"Missing segments ({len(ms)} of {inst.N}): {sample}{' ...' if len(ms)>50 else ''}")
        print("\n=== RESULT ===")
        print("Feasible cover: NO")
        print("Total cost: INF")
        print("Influencers used (indices): —")
        print(f"Segments covered: 0 / {inst.getNumSegments()}")
        return 1
    if args.algo == "bf":
        solver = BruteForceSolver()
        sol = solver.solve(inst, Solution(), 0)
    else:
        solver = BacktrackingSolver()
        best = Solution(); best.setCost(INF)
        sol = solver.solve(inst, Solution(), 0, best)
    feasible = sol.getCost() < INF and sol.getCoveredCount() == inst.getNumSegments()
    print("\n=== RESULT ===")
    print("Feasible cover:", "YES" if feasible else "NO")
    print("Total cost:", sol.getCost() if feasible else "INF")
    print("Influencers used (indices):", sol.getUsedInfluencers() if feasible else "—")
    print("Segments covered:", sol.getCoveredCount(), "/", inst.getNumSegments())
    if args.audit and feasible:
        covered = set()
        for i in sol.getUsedInfluencers():
            _, segs = inst.getInfluencer(i)
            covered |= segs
        print("Covered segments count:", len(covered))
        if len(covered) <= 200:
            print("Covered segments:", sorted(covered))
    return 0 if feasible else 1

# --------------------------
# Main
# --------------------------
def main(argv: List[str]) -> int:
    p = argparse.ArgumentParser(description="Influencer Cover (Python)")
    mode = p.add_mutually_exclusive_group(required=False)
    mode.add_argument("--file", help="Path to instance file")
    mode.add_argument("--csv-in", help="Path to input CSV to recompute")
    p.add_argument("--csv-out", help="Path to output CSV (required with --csv-in)")
    p.add_argument("--instances-dir", help="Base dir to prepend to Instancia paths in CSV")
    p.add_argument("--algo", choices=["bf","bt"], default="bt", help="Algorithm: brute force or backtracking")
    p.add_argument("--verbose", action="store_true")
    p.add_argument("--audit", action="store_true", help="Print diagnostics about feasibility and covered segments")
    p.add_argument("--variant", choices=["all","micro","macro"], default="all",
                   help="Solve on all influencers (default), or only 'micro' (<K) or only 'macro' (>=K)")
    p.add_argument("--k", type=int, help="K threshold for micro/macro (required if variant != 'all')")
    args = p.parse_args(argv)

    # CSV mode
    if args.csv_in:
        if not args.csv_out:
            print("Error: --csv-out is required when using --csv-in.", file=sys.stderr); return 1
        recompute_csv(args.csv_in, args.csv_out, args.instances_dir)
        print(f"Wrote {args.csv_out}")
        return 0

    # Instance mode
    if not args.file:
        print("Error: either --file or --csv-in must be provided.", file=sys.stderr); return 1
    if args.variant != "all" and (args.k is None or args.k <= 0):
        print("Error: --k must be provided and >0 when using --variant micro|macro.", file=sys.stderr)
        return 1
    return run(args)

if __name__ == "__main__":
    # Si NO pasás argumentos -> modo default: regenerar CSV
    if len(sys.argv) == 1:
        try:
            recompute_csv("resultados.csv", "resultados_fixed.csv", instances_dir=".")
            print("Escribí resultados_fixed.csv con los costos/selecciones corregidos")
            sys.exit(0)
        except FileNotFoundError as e:
            print(f"Error: {e}. Asegurate de correrlo donde está resultados.csv.", file=sys.stderr)
            sys.exit(1)
    # Si hay argumentos -> usa el CLI normal (instancia o --csv-in/--csv-out)
    sys.exit(main(sys.argv[1:]))
