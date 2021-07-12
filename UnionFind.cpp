#include "UnionFind.h"

UnionFind::UnionFind(int n) : _altura(n), _padre(n, 0) {
	for (int i = 0; i < n; i++) {
		_altura[i] = 1;
		_padre[i] = i;
	}
}

int UnionFind::find(int x) {
	if (_padre[x] != x)
		_padre[x] = find(_padre[x]);
	
	return _padre[x];
}

void UnionFind::uni(int x, int y) {
	x = find(x); y = find(y);
	
	if (_altura[x] < _altura[y]) {
		_padre[x] = y;
	} else {
		_padre[y] = x;
	}

	if (_altura[x] == _altura[y])
		_altura[x]++;
}