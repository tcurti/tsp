#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <utility>
#include <map>
#include "heuristicas.h"

using namespace std;

int main(int argc, char** argv) {
	
	map<string, string> heuristicas_implementadas = {
		{"G-VMC", "Golosa por vecino mas cercano"},
		{"G-AMC", "Golosa por arista mas corta"},
		{"AGM", "Basada en arbol generador minimo"},
		{"TSS", "Tabu Search, memoria basada en ultimas soluciones exploradas"},
		{"TSE", "Tabu Search, memoria basada en estructura"}
	};

	if (argc < 2) {
		cerr << "Solicite alguna heurisitca." << endl;
		cerr << "Las heuristicas disponibles son: " << endl;
		for (auto& h_desc: heuristicas_implementadas) cerr << "\t- " << h_desc.first << ": " << h_desc.second << endl;
		return 1;
	}

	// Verificamos que la heuristica pedida sea valida
	string heuristica = argv[1];

	if (heuristicas_implementadas.find(argv[1]) == heuristicas_implementadas.end()){
		cerr << "La heuristica solicitada no fue encontrada: " << argv[1] << endl;
		cerr << "Las heuristicas disponibles son: " << endl;
		for (auto& h_desc: heuristicas_implementadas) cerr << "\t- " << h_desc.first << ": " << h_desc.second << endl;
		return 1;
	}
	
	// Registramos el input
	int n, m;
	cin >> n >> m;

	vector<pair<int,int>> E(m);
	vector<vector<int>> C(n+1,vector<int>(n+1,0)); // Los vertices estan en {1,...,n}
		for (int k = 0; k < m; k++) {
			int i, j, c;
			cin >> i >> j >> c;    	
			E[k] = make_pair(i,j);
			C[i][j] = C[j][i] = c;
		}

	// Ejecutamos la heursitica y cronometamos su ejecución
	vector<int> circuito;
	auto t_ini = chrono::steady_clock::now();

	if (heuristica == "G-VMC") {
		Grafo G = {n, m, E, C};
		circuito = heurVecinos(&G).second;
	}
	else if (heuristica == "G-AMC") {
		circuito = aristaMasCorta(n, E, C);
	}
	else if (heuristica == "AGM") {
		circuito = heurAG(n, C);
	} 
	else if (heuristica == "TSS") {
		if (argc > 2) {
				tabuMaxSizeUs = stoi(argv[2]);
				maxIterUs = stoi(argv[3]);
			}
		circuito = heuristicaUs(C);
	} 
	else if (heuristica == "TSE") {
		// Experimental: eleccion de parametros
		if (argc > 2) {
			tabuMaxSize = stoi(argv[2]);
			maxIter = stoi(argv[3]);
		}
		circuito = tabuAristas(n, C);
	}

	auto t_fin = chrono::steady_clock::now();
	double t_total = chrono::duration<double, milli>(t_fin - t_ini).count();

	// Imprimimos el tiempo de ejecución por stderr.
	clog << t_total << endl;

	// Imprimimos el resultado por la salida standard.
		cout << n << " " << costoCamino(C, circuito) << endl;
		for (auto v : circuito) cout <<  v << " ";
		cout << endl;
    return 0;
}