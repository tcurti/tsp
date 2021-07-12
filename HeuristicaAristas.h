#ifndef ALGO3_TP2_HEURISTICAARISTAS_H
#define ALGO3_TP2_HEURISTICAARISTAS_H

#include <algorithm>
#include <utility>
#include <map>
#include <vector>
#include <list>

using namespace std;

// Definicion de constantes
#define TABU_MAX_SIZE    100
#define MAX_ITER        100

// Definicion de variables globales
// Experimental: eleccion de parametros
extern int tabuMaxSize;
extern int maxIter;

// Definicion de tipos
typedef pair<int, int> arista;

struct Solucion {
    int costo;
    vector<int> camino;
};

struct Tabu {
    int max_size;
    list<pair<arista, arista>> memoria;
};

// Metaheuristica tabu con memoria basada en estructura
vector<int> tabuAristas(int n, vector<vector<int>> &C);

// Definicion de funciones auxiliares
vector<int> solucionTrivial(int n);

int costoCamino(vector<vector<int>> &C, vector<int> &p);

vector<pair<arista, arista>> generarMovs2opt(vector<int> &p);

bool esMovTabu(Tabu &T, pair<arista, arista> &m);

pair<arista, arista> movOrdenado(pair<arista, arista> m);

bool aspiracionPermite(Solucion &S, Solucion &s, vector<vector<int>> &C, pair<arista, arista> &m);

int costoMov(vector<vector<int>> &C, pair<arista, arista> &m);

void recordarMov(Tabu &T, pair<arista, arista> &m);

void opt2Swap(vector<int> &s, pair<arista, arista> &m);

#endif //ALGO3_TP2_HEURISTICAARISTAS_H