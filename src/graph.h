#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <stack>

class Graph {
private:
    int n; // number of vertices
    std::vector<std::vector<int>> adj; // adjacency list
public:
    Graph(int n);
    void addEdge(int u, int v);
    const std::vector<int>& getNeighbors(int u) const;
    int getNumVertices() const;
};

#endif // GRAPH_H
