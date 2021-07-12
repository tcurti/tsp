#ifndef ALGO3_TP2_HEURISTICAAGM_H
#define ALGO3_TP2_HEURISTICAAGM_H

#include <vector>
#include <list>
#include <stack>
#include <limits>

using namespace std;

// Definicion de tipos
typedef pair<int, int> arista;

vector<int> heurAG(int n, vector<vector<int>>& C);

// Declaracion de funciones auxiliares
vector<int> primAGM(int n, vector<vector<int>>& C);
int vecinoMasCercanoNoVisitado(vector<bool>& visitado, vector<int>& distancia);
vector<list<int>> listaAdyacenciaAGM(vector<int>& padre);
vector<int> ordenDFS(vector<list<int>>& G);
vector<int> secCircuitoSiguiendoOrden(vector<int>& orden);

#endif //ALGO3_TP2_HEURISTICAAGM_H
