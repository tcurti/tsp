#include "HeuristicaAristas.h"

// Definicion de variables globales
// Experimental: eleccion de parametros
int tabuMaxSize = TABU_MAX_SIZE;
int maxIter = MAX_ITER;

vector<int> tabuAristas(int n, vector<vector<int>>& C) {
	Solucion s = { 0, solucionTrivial(n)}; // TODO: solucion inicial
	s.costo = costoCamino(C, s.camino);
	Solucion S = s; // Mejor solucion hallada
	Tabu T = {tabuMaxSize, {}}; // Lista tabu
	
	for (int i = 0; i < maxIter; i++) { // Criterio de corte: cantidad de iteraciones
		vector<pair<arista, arista>> M = generarMovs2opt(s.camino); // Movimientos que generan la subvecindad

		// Elegimos el mejor movimiento en M
		// Si todos los movimientos son tabu, usamos el primero de M
		pair<arista,arista> m_prima = M[0];
		bool hallamosNoTabu = false;
		for (auto& m : M) {
			// No debe ser tabu o debe estar permitido por la funcion de aspiracion
			if (!esMovTabu(T, m) || aspiracionPermite(S, s, C, m)) { 
				if (!hallamosNoTabu) {
					hallamosNoTabu=true;
					m_prima = m;
				}
				// Comparamos el costo del movimiento actual con el del mejor hasta el momento
				// De ser apropiado, actualizamos el mejor mov. hallado
				if (costoMov(C, m) < costoMov(C, m_prima)) 
					m_prima = m;
			}
		}

		// Actualizamos la lista tabu
		if ( !esMovTabu(T, m_prima) ) 
			recordarMov(T, m_prima);

		// Realizamos el mejor movimiento hallado
		opt2Swap(s.camino, m_prima);
		s.costo += costoMov(C, m_prima);

		// De ser apropiado, actualizamos la mejor solucion hallada
		if (s.costo < S.costo)
			S = s;
	}

	return S.camino;
}

vector<int> solucionTrivial(int n) {
	vector<int> s = {};
	for (int i = 1; i <= n; i++)
		s.push_back(i);

	return s;
}

int costoCamino(vector<vector<int>>& C, vector<int>& p) {
	int c = 0;
	for (int i = 0; i < p.size(); i++) {
		c += C[p[i]][p[(i+1)%p.size()]];
	}

	return c;
}

vector<pair<arista, arista>> generarMovs2opt(vector<int>& p) {
	vector<pair<arista, arista>> M = {};

	int n = p.size();
	for (int i = 0; i < n-1; i++) {
		arista e = make_pair(p[i], p[i+1]);

		for (int j = i+2; j < n; j++) { // j=i+2 evita agregar el mov. ((a,b), (b,c))
			arista f = make_pair(p[j], p[(j+1)%n]);
			if (e.first != f.second) M.push_back(make_pair(e,f)); // evitamos agregar ((1,b),(c,1))
		}
	}

	return M;
}

bool esMovTabu(Tabu& T, pair<arista,arista>& m) {			
	return find( T.memoria.begin(), T.memoria.end(), movOrdenado(m) ) != T.memoria.end();
}

// m=((a,b),(c,d)) ordenado tq: a<b, c<d, a<=c y (a=c->b<d)
pair<arista,arista> movOrdenado(pair<arista,arista> m) {
	// Ordenamos la primera arista
	if (m.first.first > m.first.second) {
		int v_aux = m.first.second;
		m.first.second = m.first.first;
		m.first.first = v_aux;
	}
	// Ordenamos la segunda arista
	if (m.second.first > m.second.second) {
		int v_aux = m.second.second;
		m.second.second = m.second.first;
		m.second.first = v_aux;
	}
	// Ordenamos el par de aristas
	if (m.first.first > m.second.first) {
		arista e_aux = m.second;
		m.second = m.first;
		m.first = e_aux;
	}
	else if( (m.first.first == m.second.first) && (m.first.second > m.second.second) ) {
		int v_aux = m.second.second;
		m.second.second = m.first.second;
		m.first.second = v_aux;
	}

	return m;
}

bool aspiracionPermite(Solucion& S, Solucion& s, vector<vector<int>>& C, pair<arista, arista>& m) {
	// Permitimos movimientos que generen soluciones mejores que la optima hallada hasta el momento
	return S.costo > s.costo + costoMov(C, m);
}

int costoMov(vector<vector<int>>& C, pair<arista,arista>& m) {
	arista e = m.first; arista f = m.second;

	int c_actual = C[e.first][e.second] + C[f.first][f.second];
	int c_swap = C[e.first][f.first] + C[e.second][f.second];

	return c_swap - c_actual;
}

// Pre: |T.memoria| < T.max_
void recordarMov(Tabu& T, pair<arista,arista>& m) {
	// Agregamos el movimiento a la memoria tabu
	T.memoria.push_back(movOrdenado(m));
	
	// Si superamos la maxima capacidad, quitamos el elemento mas viejo
	if (T.memoria.size() == T.max_size)
		T.memoria.pop_front();
		
}

// Pre: sea m=((v_i,v_i+1),(v_k,v_k+1)), entonces i<k
void opt2Swap(vector<int>& s, pair<arista,arista>& m) {
	arista e = m.first; arista f = m.second;
	reverse( find(s.begin(),s.end(), e.second), find(s.begin(), s.end(), f.first)+1 );
}