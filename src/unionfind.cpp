#include "unionfind.h"

UnionFind::UnionFind(int _n) : root(_n) {
    for(int i = 0; i < _n; i++) root[i] = i;
}

int UnionFind::findRoot(int x) {
    return x == root[x] ? x : root[x] = findRoot(root[x]);
}

void UnionFind::unite(int x, int y) {
    int x_root = findRoot(x), y_root = findRoot(y);
    root[x_root] = y_root;
}

void UnionFind::setSize(int n) {
    root.resize(n);
    for (int i = 0; i < n; i++) {
        root[i] = i;
    }
}
