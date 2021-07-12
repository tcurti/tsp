#ifndef __UNION_FIND_H__
#define __UNION_FIND_H__

#include <vector>

using namespace std;

class UnionFind {
	public:
		UnionFind(int n);

		int find(int x);
		void uni(int x, int y);

	private:
		vector<int> _altura;
		vector<int> _padre;
};

#endif //__UNION_FIND_H__