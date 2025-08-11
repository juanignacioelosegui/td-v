import unittest
from Esqueleto import grafo_completo, grafo_bipartito_completo, grafo_rueda


class TestGrafoCompleto(unittest.TestCase):
    def test_grafo_completo_con_un_vertice(self):
        matriz = grafo_completo(1)
        self.assertEqual(1, len(matriz))
        self.assertEqual(1, len(matriz[0]))
        self.assertTrue(self._chequear_si_grafo_es_completo(1, matriz))
    
    def test_grafo_completo_con_dos_vertices(self):
        matriz = grafo_completo(2)
        self.assertEqual(2, len(matriz))
        self.assertEqual(2, len(matriz[0]))
        self.assertTrue(self._chequear_si_grafo_es_completo(2, matriz))

    def test_grafo_completo_con_muchos_vertices(self):
        matriz = grafo_completo(100)
        self.assertEqual(100, len(matriz))
        self.assertEqual(100, len(matriz[0]))
        self.assertTrue(self._chequear_si_grafo_es_completo(100, matriz))

    def _chequear_si_grafo_es_completo(self, n: int, matriz: list[list[int]]) -> bool:
        diagonal_es_cero = all([matriz[i][i] == 0 for i in range(n)])
        resto_es_uno = all([matriz[i][j] for i in range(n) for j in range(n) if i != j])
        return diagonal_es_cero and resto_es_uno

'''
class TestGrafoBipartitoCompleto(unittest.TestCase):
    def test_grafo_bipartito_completo_con_un_vertice_de_cada_lado(self):
        matriz = grafo_bipartito_completo(1, 1)
        self.assertEqual(2, len(matriz))
        self.assertEqual(2, len(matriz[0]))
        self.assertTrue(self._chequear_si_grafo_es_bipartito_completo(1, 1, matriz))

    def test_grafo_bipartito_completo_con_un_vertice_a_la_izquierda_y_muchos_a_la_derecha(
        self,
    ):
        matriz = grafo_bipartito_completo(1, 100)
        self.assertEqual(101, len(matriz))
        self.assertEqual(101, len(matriz[0]))
        self.assertTrue(self._chequear_si_grafo_es_bipartito_completo(1, 100, matriz))

    def test_grafo_bipartito_completo_con_muchos_vertices_a_la_izquierda_y_uno_a_la_derecha(
        self,
    ):
        matriz = grafo_bipartito_completo(100, 1)
        self.assertEqual(101, len(matriz))
        self.assertEqual(101, len(matriz[0]))
        self.assertTrue(self._chequear_si_grafo_es_bipartito_completo(100, 1, matriz))

    def test_grafo_bipartito_completo_con_muchos_vertices_a_la_izquierda_y_muchos_a_la_derecha(
        self,
    ):
        matriz = grafo_bipartito_completo(100, 100)
        self.assertEqual(200, len(matriz))
        self.assertEqual(200, len(matriz[0]))
        self.assertTrue(self._chequear_si_grafo_es_bipartito_completo(100, 100, matriz))

    def _chequear_si_grafo_es_bipartito_completo(
        self, n: int, m: int, matriz: list[list[int]]
    ):
        dentro_de_particion_es_cero = all(
            matriz[i][j] == 0 for i in range(n) for j in range(n)
        ) and all(matriz[i][j] == 0 for i in range(n, n + m) for j in range(n, n + m))
        entre_particiones_es_uno = all(
            matriz[i][j] == 1 and matriz[j][i] == 1
            for i in range(n)
            for j in range(n, n + m)
        )
        return dentro_de_particion_es_cero and entre_particiones_es_uno


class TestGrafoRueda(unittest.TestCase):
    def test_grafo_rueda_de_cuatro_nodos(self):
        matriz = grafo_rueda(4)
        self.assertEqual(4, len(matriz))
        self.assertEqual(4, len(matriz[0]))
        self.assertTrue(self._chequear_si_grafo_es_rueda(4, matriz))

    def test_grafo_rueda_de_muchos_nodos(self):
        matriz = grafo_rueda(100)
        self.assertEqual(100, len(matriz))
        self.assertEqual(100, len(matriz[0]))
        self.assertTrue(self._chequear_si_grafo_es_rueda(100, matriz))

    def _chequear_si_grafo_es_rueda(self, n: int, matriz: list[list[int]]):
        diagonal_es_cero = all([matriz[i][i] == 0 for i in range(n)])
        ultimo_conectado_con_todos = all(
            [matriz[n - 1][i] == 1 and matriz[i][n - 1] == 1 for i in range(n - 1)]
        )
        conectados_en_forma_de_ciclo = all(
            [
                matriz[i][(i + 1) % (n - 1)] == 1 and matriz[(i + 1) % (n - 1)][i] == 1
                for i in range(n - 1)
            ]
        )

        resto_no_conectados = all(
            [
                matriz[i][j] == 0
                for i in range(n - 1)
                for j in range(n - 1)
                if (i + 1) % (n - 1) != j and (j + 1) % (n - 1) != i
            ]
        )

        return (
            diagonal_es_cero
            and ultimo_conectado_con_todos
            and conectados_en_forma_de_ciclo
            and resto_no_conectados
        )
'''

if __name__ == "__main__":
    unittest.main()
