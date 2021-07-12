#include "HeuristicasGolosas.h"


// Vecino más cercano
/**
 * Devuelve el vértices vecino más cercano a v.
 * @param vecinos Vector de vértices adyacentes a v.
 * @param v Vértice cuyo vecino más cercano quiero.
 * @param H Vector de vértices insertados hasta el momento.
 * @return Vértice adyacente más cercano a v
 */
int masCercano(std::vector<int> *vecinos, int v, std::vector<int> *H) {
    int res = 0;
    int min = std::numeric_limits<int>::max();      // Inicializo min con "infinito"
    for (int i = 2; i < vecinos->size(); i++)
        if ( !( std::find(H->begin(), H->end(), i) != H->end() ) and (*vecinos)[i] < min) {
            // Actualizo el mínimo sí:
            //      - el vecino i no está ya en el circuito
            //      - si el peso de la arista (v, i) < min
            min = (*vecinos)[i];
            res = i;
        }
    return res;
}

/**
 * Devuelve un circuito hamiltoniano de costo mínimo del grafo G utilizando la heurística del vecino más cecano.
 * @param G Grafo cuyo circuito quiero encontrar.
 * @return par<a,b>, donde a := costo del circuito, y b := circuito de vértices.
 */
std::pair<int, std::vector<int>> heurVecinos(Grafo *G) {
    int costo = 0;
    int v = 1;
    std::vector<int> H = {1};
    while (H.size() < G->n) {
        int w = masCercano(&G->M[v], v, &H);
        costo += G->M[v][w];
        H.push_back(w);
        v = w;
    }
    return std::make_pair(costo + G->M[H[0]][H[H.size() - 1]], H);
}


// Arista más corta.
/**
 * Formatea la información del vector de aristas para devolver un par con el costo y el circuito de vértices.
 * Ordena la secuencia de aristas del circiuto tal que
 *                                (\forall x,y:arista) ( y sigue inmediatamente a x => x.snd == y.fst )
 * y luego recorre dicha secuencia sumando al resultado el costo de cada arista y agregando al vector de vértices del
 * circuito, la primera componente de cada arista.
 * @param pGrafo Puntero al Grafo.
 * @param pVector Puntero al vector de aristas del circiuto hamiltoniano encontrado.
 * @return par<a,b>, donde a := costo del circuito, y b := circuito de vértices.
 */
std::pair<int, std::vector<int>> formatResult(Grafo *pGrafo, std::vector<std::pair<int, int>> *pVector) {
    std::vector<std::pair<int, int>> x_t = {(*pVector)[0]};
    pVector->erase(pVector->begin());
    for (int k = 0; !pVector->empty(); k++) {
        auto e = std::find_if(
                pVector->begin(),
                pVector->end(),
                [&](auto e) { return (e.first == x_t[k].second or e.second == x_t[k].second) and (e != x_t[k]); });
        x_t.emplace_back(x_t[k].second == e->first ? (*e) : std::make_pair(e->second, e->first));
        pVector->erase(e);
    }
    std::vector<int> v;
    int costo = 0;
    std::for_each(x_t.begin(), x_t.end(), [&](auto e) {
        v.push_back(e.first);
        costo += pGrafo->M[e.first][e.second];
    });
    return std::make_pair(costo, v);
}

/**
 * Devuelve un circuito hamiltoniano de costo mínimo del grafo G utilizando la heurística de la arista más corta.
 * @param G Grafo cuyo circuito quiero encontrar.
 * @return par<a,b>, donde a := costo del circuito, y b := circuito de vértices.
 */
/*std::pair<int, std::vector<int>> heurAristaMasCorta(Grafo *G) {
    std::vector<std::pair<int, int>> X = G->X;
    // Ordeno las aristas por peso.
    std::sort(X.begin(), X.end(), [&](auto a, auto b) { return G->M[a.first][a.second] < G->M[b.first][b.second]; });

    // grados := vector para llevar cuenta de los grados de los vértices.
    std::vector<int> grados(G->n + 1, 0);

    int i = 1;
    // X_t := aristas del circuito.
    std::vector<std::pair<int, int>> X_t;
    while (i < G->n) {
        // Elimino las aristas incidentes a los vértices saturados.
        X.erase(
                std::remove_if(
                        X.begin(),
                        X.end(),
                        [&](auto &elem) {
                            return grados[elem.first] == 2 or grados[elem.second] == 2;
                        }
                ),
                X.end()
        );
        // Agrego la arista de menor costo posible, la elimino de las aristas de G y actualizo el grado de los
        // vértices que une.
        std::pair<int, int> e = X[0];
        X.erase(X.begin());
        X_t.emplace_back(e);
        grados[e.first]++;
        grados[e.second]++;
        i++;
    }
    // Agrego la arista faltante (la incidente a los únicos dos vértices de grado 1).
    int a = 0, b = 0;
    for (int k = 0; k < grados.size(); k++) {
        if (grados[k] == 1) {
            if (a == 0) a = k;
            else b = k;
        }
    }
    X_t.emplace_back(std::make_pair(a, b));

    return formatResult(G, &X_t);
}*/

vector<int> aristaMasCorta(int n, vector<arista> E, vector<vector<int>>& C) {
    vector<vector<int>> circuito(n+1);      // Lista de adyacencia; facilita la conversion a secuencia
    vector<int> grados_circuito(n+1, 0);    // Usamos n+1 para indexar directamente, ya que V={1,..,n}
    UnionFind componentes_circuito(n+1);
    ordenarPorPeso(E, C);
    
    for (auto& e : E) {
        bool forma_ciclo = componentes_circuito.find(e.first) == componentes_circuito.find(e.second);
        
        if (!forma_ciclo && grados_circuito[e.first]<=1 && grados_circuito[e.second]<=1) {
            // Agregamos la arista e al circuito
            circuito[e.first].push_back(e.second);
            circuito[e.second].push_back(e.first);
            // Actualizamos componentes conexas y grados
            componentes_circuito.uni(e.first, e.second);
            grados_circuito[e.first]++; grados_circuito[e.second]++;
        }
    }

    cerrarCircuito(circuito, grados_circuito);
    return secuenciaVertices(circuito);
}

void ordenarPorPeso(vector<arista>& E, vector<vector<int>>& C) {
    // Ordenamos el vector de aristas E segun su peso determinado por C
    sort(E.begin(), E.end(), 
        [&](auto i, auto j) { 
        return C[i.first][i.second] < C[j.first][j.second]; 
    });
}

// Representacion camino: lista de adyacencia
// Facilita la posterior conversion a secuencia.
void cerrarCircuito(vector<vector<int>>& camino, vector<int>& grados) {
    // Buscamos dos vertices i, j talque su grado sea 1.
    int i = 0, j = 0;
    for (int k = 1; k < grados.size(); k++) {
        if (grados[k] == 1) {
            if (i == 0) {
                i = k;
            } else {
                j = k;
            }
        }
    }

    // Si agregamos la arista (i,j) cerramos el circuito
    camino[i].push_back(j); camino[j].push_back(i);
    grados[i]++; grados[j]++;
}

// Representacion circuito: lista de adyacencia
// Facilita la conversion a secuencia.
vector<int> secuenciaVertices(vector<vector<int>>& circuito) {
    vector<int> vertices = {};
    
    // Comenzamos por el vertice 1
    vertices.push_back(1);

    int i = circuito[1][0]; // i = "algun vecino de 1"
    while (i != 1) { // cuando el vecino sea 1, habremos dado toda la vuelta
        // Elegimos el vecino de i que aun no exploramos
        vector<int> vecinos_i = circuito[i];
        int prox_vecino = (vecinos_i[0] != vertices.back())? vecinos_i[0] : vecinos_i[1];
        
        // Agregamos i a la secuencia y avanzamos a su vecino
        vertices.push_back(i);
        i = prox_vecino;
    }

    return vertices;
}