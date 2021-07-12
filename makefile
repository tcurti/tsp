all: 
	g++ -o tsp tsp.cpp heuristicas.cpp UnionFind.cpp HeuristicasGolosas.cpp HeuristicaAGM.cpp HeuristicaAristas.cpp HeuristicaUS.cpp -O2 -std=c++14