#ifndef ALGO3_TP2_HEURISTICASGOLOSAS_H
#define ALGO3_TP2_HEURISTICASGOLOSAS_H

#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <algorithm>
#include "UnionFind.h"

using namespace std;

struct Grafo {
    int n;
    int m;
    std::vector<std::pair<int, int>> X;
    std::vector<std::vector<int>> M;
};

// Definicion de tipos
typedef pair<int, int> arista;

// Declaracion de heuristicas
std::pair<int, std::vector<int>> heurVecinos(Grafo*);
std::pair<int, std::vector<int>> heurAristaMasCorta(Grafo*);
vector<int> aristaMasCorta(int n, vector<arista> E, vector<vector<int>>& C);

// Declaracion de funciones auxiliares
void ordenarPorPeso(vector<arista>& E, vector<vector<int>>& C);
void cerrarCircuito(vector<vector<int>>& camino, vector<int>& grados);
vector<int> secuenciaVertices(vector<vector<int>>& aristas_circuito);

#endif //ALGO3_TP2_HEURISTICASGOLOSAS_H
