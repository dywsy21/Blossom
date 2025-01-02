#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>

class UnionFind {
private:
    std::vector<int> root;
public:
    UnionFind() {}
    UnionFind(int n);
    int findRoot(int x);
    void unite(int x, int y);
    void setSize(int n);
};

#endif // UNIONFIND_H
