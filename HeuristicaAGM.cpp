#include "HeuristicaAGM.h"

vector<int> heurAG(int n, vector<vector<int>>& C) {
	vector<int> padres_agm = primAGM(n, C);

	vector<list<int>> ady_agm = listaAdyacenciaAGM(padres_agm);

	vector<int> orden_dfs_agm = ordenDFS(ady_agm);

	vector<int> circuito_hamil = secCircuitoSiguiendoOrden(orden_dfs_agm);

	return circuito_hamil;
}

vector<int> primAGM(int n, vector<vector<int>>& C) {
	vector<bool> visitado(n+1, false);	//Usamos n+1 para indexar directamente, ya que V={1,..,n}
	vector<int> distancia(n+1, numeric_limits<int>::max());
	vector<int> padre(n+1, -1);

	int i = 1; // Tomamos s=1 como nodo inicial
	visitado[i] = true;
	distancia[i] = 0;
	padre[i] = 0;
	for (int j = 2; j <= n; j++) {
		distancia[j] = C[i][j];
		padre[j] = i;
	}

	int cant_visitados = 1;
	while (cant_visitados < n) {
		int v = vecinoMasCercanoNoVisitado(visitado, distancia);

		for (int w = 1; w <= n; w++) {
			bool es_sucesor = (w != v) && !visitado[w];
			if ( es_sucesor && (distancia[w] > C[v][w]) ) {
				distancia[w] = C[v][w];
				padre[w] = v;
			} 
		}

		visitado[v] = true;
		cant_visitados++;
	}

	return padre;
}

int vecinoMasCercanoNoVisitado(vector<bool>& visitado, vector<int>& distancia) {
	int arg_min_distancia = 1;
	for (int v = 1; v < visitado.size(); v++) {
		if ( !visitado[v] && (distancia[v] < distancia[arg_min_distancia]) )
			arg_min_distancia = v;
	}

	return arg_min_distancia;
}

// Representacion AGM entrada: padre[1,..n] = padre del nodo en el AGM, padre=0 si raiz
vector<list<int>> listaAdyacenciaAGM(vector<int>& padre) {
	vector<list<int>> T(padre.size());

	for (int i = 1; i < padre.size(); i++) {
		bool es_raiz = padre[i] == 0;
		if (!es_raiz) {
			T[i].push_back( padre[i] );
			T[ padre[i] ].push_back(i);
		}
	}

	return T;
}

// Representacion de G: list de adyacencia
vector<int> ordenDFS(vector<list<int>>& G) {
	int n = G.size()-1;
	vector<bool> visitado(n+1, false); //Usamos n+1 para indexar directamente, ya que V={1,..,n}
	vector<int> orden(n+1, 0);
	stack<int> vertices;
	int cant_visitados = 0;

	// Comenzamos por el vertice 1
	visitado[1] = true;
	cant_visitados++;
	orden[1] = cant_visitados;
	vertices.push(1);

	while (!vertices.empty()) {
		int v = vertices.top(); vertices.pop();

		for (auto w : G[v]) {
			if (!visitado[w]) {
				visitado[w] = true;
				cant_visitados++;
				orden[w] = cant_visitados;
				vertices.push(w);
			}
		}
	}

	return orden;
}

// Los ordenes del recorrido van de 1, ..., n
vector<int> secCircuitoSiguiendoOrden(vector<int>& orden) {
	int n = orden.size()-1; // orden indexa directamente sobre [1,...,n]
	vector<int> sec_circuito(n);

	for (int i = 1; i <= n; i++)
		sec_circuito[ orden[i]-1 ] = i;
	
	return sec_circuito;
}