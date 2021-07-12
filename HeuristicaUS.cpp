#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <random>
using namespace std;

int tabuMaxSizeUs = 100;
int maxIterUs = 5;
/*
 * logica interna que se utiliza en este problema
 * los swaps estan definidos siempre como el nodo y su siguiente, por lo tanto
 * si un camino es  1-5-8-6-9-4-2-3 el swap de la arista 5-8 a la 4-2
 * se escribe como ((5,8),(4,2))
 */

/* devuelve el cambio al ciclo que el swap generaria */
int swapCost(pair<int, int> inicio, pair<int, int> fin,
             const vector<vector<int>> &aristas) {
  pair<int, int> nuevoInicio = make_pair(inicio.first, fin.second);
  pair<int, int> nuevoFin = make_pair(fin.first, inicio.second);
  int diferencia =
      aristas[nuevoInicio.first][nuevoInicio.second] +
      aristas[nuevoFin.first][nuevoFin.second] -
      (aristas[inicio.first][inicio.second] + aristas[fin.first][fin.second]);
  return diferencia;
}

// comparo 2 pares sin importar el orden interno
template <typename T> bool comparePair(pair<T, T> a, pair<T, T> b) {
  return (a.first == b.first && a.second == b.second) ||
         (a.first == b.second && a.second == b.first);
}

// comparo 2 swap sin tener en cuenta el orden interno
bool compareSwap(pair<pair<int, int>, pair<int, int>> a,
                 pair<pair<int, int>, pair<int, int>> b) {
  return comparePair(a.first, b.first) && comparePair(a.second, b.second) ||
         comparePair(a.first, b.second) && comparePair(a.second, b.first);
}

vector<pair<pair<int, int>, pair<int, int>>>
opt2Us(const vector<int> &camino, vector<vector<int>> &aristas) {
  vector<pair<pair<int, int>, pair<int, int>>> result;
  int n = camino.size();
  for (int i = 0; i < n - 1; i++) {
    pair<int, int> e = make_pair(camino[i], camino[i + 1]);

    for (int j = i + 2; j < n;
         j++) { // j=i+2 evita agregar el mov. ((a,b), (b,c))
      pair<int, int> f = make_pair(camino[j], camino[(j + 1) % n]);
      if (e.first != f.second)
        result.push_back(make_pair(e, f)); // evitamos agregar ((1,b),(c,1))
    }
  }
  // sacaria los duplicados
  // result.erase(unique(result.begin(), result.end(), compareSwap),
  // result.end()); seteo un random generador para el sampleo
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> disti(0, result.size() - 1);

  vector<pair<pair<int, int>, pair<int, int>>> sample;
  for (int i = 0; i < int(result.size() * 0.3); ++i) {
    pair<pair<int, int>, pair<int, int>> b = result[disti(gen)];
    if (find(sample.begin(), sample.end(), b) == sample.end()) {
      sample.push_back(b);
    }
  }

  for (int i = 0; i < 10; ++i) {
    pair<pair<int, int>, pair<int, int>> minAcum = result[0];
    for (auto &j : result) {
      if (swapCost(j.first, j.second, aristas) <
              swapCost(minAcum.first, minAcum.second, aristas) &&
          find(sample.begin(), sample.end(), j) == sample.end()) {
        minAcum = j;
      }
    }
    sample.push_back(minAcum);
  }
  return sample;
}

inline int modulo(int a, int b) {
  const int result = a % b;
  return result >= 0 ? result : result + b;
}

/*
 * me fijo si alguna de las aristas es la del fin con el inicio
 * si lo es, muevo el inicio de la lista a la cola, de esta manera siempre puedo
 * asegurar que hay un segmento entero de las aristas osea no tengo algo de este
 * esilo 5-8-0-9-4-6-7, camino hamiltoniano, y me piden el swap ((0-9),(7-5))
 * cualdo busque los indices, se transforma en ((2-3).(6-0))
 * que es cosiderablemente mas dificl de encontrar el segmento que transformando
 * el camino a 8-0-9-4-6-7-5, con los inidces ((1-2),(5-6))
 */

void makeSwap(vector<int> &camino, pair<pair<int, int>, pair<int, int>> swap) {
  int indexA =
      find(camino.begin(), camino.end(), swap.first.first) - camino.begin();
  int indexB =
      find(camino.begin(), camino.end(), swap.first.second) - camino.begin();
  int indexC =
      find(camino.begin(), camino.end(), swap.second.first) - camino.begin();
  int indexD =
      find(camino.begin(), camino.end(), swap.second.second) - camino.begin();
  if (max(indexA, indexB) - min(indexA, indexB) != 1 ||
      max(indexC, indexD) - min(indexC, indexD) != 1) {
    reverse(camino.begin(), camino.end());
    int a = camino[camino.size() - 1];
    camino.pop_back();
    reverse(camino.begin(), camino.end());
    camino.push_back(a);
    // recalculo los indices, ya que cabiaron con el swap
    indexA = modulo(indexA - 1, camino.size());
    indexB = modulo(indexB - 1, camino.size());
    indexC = modulo(indexC - 1, camino.size());
    indexD = modulo(indexD - 1, camino.size());
  }

  int inicioIndex;
  int indexFin;
  // busco el que esta a la "izquierda"
  if (min(indexA, indexB) < min(indexC, indexD)) {
    inicioIndex = max(indexA, indexB);
    indexFin = max(indexC, indexD);
  } else {
    inicioIndex = max(indexC, indexD);
    indexFin = max(indexA, indexB);
  }
  vector<int> parcial;
  for (int i = inicioIndex; i < indexFin; ++i) {
    parcial.push_back(camino[i]);
  }
  reverse(parcial.begin(), parcial.end());
  for (int i = 0; i < indexFin - inicioIndex; ++i) {
    camino[i + inicioIndex] = parcial[i];
  }
}

int pesoDeCamino(const vector<int> &camino, vector<vector<int>> &aristas) {
  int acumulador = 0;
  // sumo todas las aristas, desde un nodo a su siguiente
  for (int i = 0; i < camino.size() - 1; ++i) {
    acumulador += aristas[camino[i]][camino[i + 1]];
  }
  // sumo el fin del cclo, desde el ultimo nodo al primero
  return acumulador + aristas[camino[camino.size() - 1]][camino[0]];
}

bool aspiracion(vector<int> &camino) {
  int chance = rand() % 100;
  if (chance < 90) {
    return false;
  } else {
    return true;
  }
}
vector<int> solucionTrivialPlaceHolder(int n) {
  vector<int> s = {};
  for (int i = 1; i <= n; i++)
    s.push_back(i);

  return s;
}

vector<int> rotador(vector<int> camino) {
  int index1 = find(camino.begin(), camino.end(), 1) - camino.end();
  vector<int> aux;
  for (int i = 0; i < camino.size(); ++i) {
    aux.push_back(camino[(i + index1) % camino.size()]);
  }
  return aux;
}

vector<int> heuristicaUs(vector<vector<int>> &aristas) {

  vector<int> solucionInicial = solucionTrivialPlaceHolder(aristas.size() - 1);
  vector<int> solucionParcial = solucionInicial;
  vector<int> mejorSolucion = solucionInicial;
  vector<vector<int>> listaTabu;
  int iteracionesSinMejora = 0;
  int iteracionesTotales = 0;
  while (iteracionesSinMejora < maxIterUs && iteracionesTotales < 5000) {
    iteracionesTotales++;
    // buscamos vecindad
    vector<pair<pair<int, int>, pair<int, int>>> vecindad =
        opt2Us(solucionParcial, aristas);
    bool foundSwap = false;
    // saco todas las tabu, menos las que permite la aspiracion
    vector<pair<pair<int, int>, pair<int, int>>> vecindadReducida;
    vecindadReducida.push_back(vecindad[0]);
    bool EncontreUna = false;
    for (auto &swap : vecindad) {
      vector<int> nuevaSolucion = solucionParcial;
      makeSwap(nuevaSolucion, swap);
      bool noEstaEnLista = find(listaTabu.begin(), listaTabu.end(),
                                nuevaSolucion) == listaTabu.end();
      bool pasaAspiracion = aspiracion(nuevaSolucion);
      if (noEstaEnLista || pasaAspiracion) {
        if (vecindadReducida.size() == 1 && !EncontreUna) {
          EncontreUna = true;
          vecindadReducida.pop_back();
        }
        vecindadReducida.push_back(swap);
      }
    }
    // busco la mejor solucion o la primera
    pair<pair<int, int>, pair<int, int>> mejorSwap = vecindadReducida[0];
    for (auto &swap : vecindadReducida) {
      if (swapCost(mejorSwap.first, mejorSwap.second, aristas) >
          swapCost(swap.first, swap.second, aristas)) {
        mejorSwap = swap;
      }
    }

    if (listaTabu.size() > tabuMaxSizeUs) {
      reverse(listaTabu.begin(), listaTabu.end());
      listaTabu.pop_back();
      reverse(listaTabu.begin(), listaTabu.end());
    }

    makeSwap(solucionParcial, mejorSwap);

    listaTabu.push_back(solucionParcial);

    if (swapCost(mejorSwap.second, mejorSwap.first, aristas) < 0) {
      iteracionesSinMejora = 0;
    } else {
      iteracionesSinMejora++;
    }
    if (pesoDeCamino(solucionParcial, aristas) <
        pesoDeCamino(mejorSolucion, aristas)) {
      mejorSolucion = solucionParcial;
    }
  }
  return rotador(mejorSolucion);
}
