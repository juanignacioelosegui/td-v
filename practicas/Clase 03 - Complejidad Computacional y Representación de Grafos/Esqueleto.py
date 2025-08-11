def grafo_completo(n: int) -> list[list[int]]:
    """Dado un número natural n devolver la matriz de adyacencia de un grafo completo con n vértices.
    Ayuda: Asumir que los nodos son {0,1,..,n-1}.
    Args:
        n (int): Cantidad de vértices del grafo.

    Returns:
        List[List[int]]: La matriz de adyacencia de un grafo completo.
    """
    matriz = [[int(i != j) for i in range(n)] for j in range(n)]
    return matriz


def grafo_bipartito_completo(n: int, m: int) -> list[list[int]]:
    """Dado dos números naturales n y m devolver la matriz de adyacencia de un grafo bipartito completo que tiene n vértices en una de
    sus particiones y m vértices en la otra.
    Asumir que los nodos de la primera particion son {0,1,..,n-1} y los de la segunda son {n,n+1,..,n+m-1}
    Args:
        n (int): Cantidad de vértices de la partición izquierda.
        m (int): Cantidad de vértices de la partición derecha.

    Returns:
        List[List[int]]: La matriz de adyacencia de un grafo bipartito completo
    """
    pass


def grafo_rueda(n: int) -> list[list[int]]:
    """Dado un natural n >= 4 devolver la matriz de adyacencia de un grado rueda de n vértices.
    Asumir que el nodo del centro es n-1 y los nodos del 0 hasta el n-2 forman en círculo en el orden 0-1-..-(n-2).
    Ayuda: https://es.wikipedia.org/wiki/Grafo_rueda
    Args:
        n (int): Cantidad de vértices del grafo.

    Returns:
        List[List[int]]: La matriz de adyacencia de un grafo rueda.
    """
    pass
