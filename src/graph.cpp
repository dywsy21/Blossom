#include "graph.h"

Graph::Graph(int n) : n(n), adj(n) {}

void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

const std::vector<int>& Graph::getNeighbors(int u) const {
    return adj[u];
}

int Graph::getNumVertices() const {
    return n;
}
