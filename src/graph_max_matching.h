#ifndef GRAPH_MAX_MATCHING_H
#define GRAPH_MAX_MATCHING_H

#include "graph.h"

class GraphSolver {
private:
    const Graph& graph;
    int n;
    std::vector<int> match, base, parent, q;
    std::vector<bool> inQueue, inBlossom;

    int lca(int a, int b);
    void markPath(int u, int b, int child);
    bool findAugmentingPath(int root);

public:
    GraphSolver(const Graph& graph);
    std::vector<int> findMaximumMatching();
};

#endif // GRAPH_MAX_MATCHING_H
