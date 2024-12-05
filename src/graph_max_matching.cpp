#include "graph_max_matching.h"
#include <algorithm>

std::vector<int> findMaximumMatching(const Graph& graph) {
    int n = graph.getNumVertices();
    std::vector<int> match(n, -1);

    std::vector<int> base(n), parent(n), q;
    std::vector<bool> inQueue(n), inBlossom(n);

    auto lca = [&](int a, int b) {
        std::vector<bool> mark(n, false);
        while (true) {
            a = base[a];
            mark[a] = true;
            if (match[a] == -1) break;
            a = parent[match[a]];
        }
        while (true) {
            b = base[b];
            if (mark[b]) return b;
            b = parent[match[b]];
        }
    };

    auto markPath = [&](int u, int b, int child) {
        for (; base[u] != b; u = parent[match[u]]) {
            inBlossom[base[u]] = inBlossom[base[match[u]]] = true;
            parent[u] = child;
            child = match[u];
        }
    };

    auto findAugmentingPath = [&](int root) {
        inQueue.assign(n, false);
        parent.assign(n, -1);
        for (int i = 0; i < n; ++i) base[i] = i;
        q.clear();
        q.push_back(root);
        inQueue[root] = true;

        for (size_t qi = 0; qi < q.size(); ++qi) {
            int v = q[qi];
            for (int u : graph.getNeighbors(v)) {
                if (base[v] == base[u] || match[v] == u) continue;
                if (u == root || (match[u] != -1 && parent[match[u]] != -1)) {
                    int curBase = lca(v, u);
                    inBlossom.assign(n, false);
                    markPath(v, curBase, u);
                    markPath(u, curBase, v);
                    for (int i = 0; i < n; ++i) {
                        if (inBlossom[base[i]]) {
                            base[i] = curBase;
                            if (!inQueue[i]) {
                                inQueue[i] = true;
                                q.push_back(i);
                            }
                        }
                    }
                } else if (parent[u] == -1) {
                    parent[u] = v;
                    if (match[u] == -1) {
                        int d = u;
                        while (d != -1) {
                            int pv = parent[d];
                            int pp = match[pv];
                            match[d] = pv;
                            match[pv] = d;
                            d = pp;
                        }
                        return true;
                    } else {
                        inQueue[match[u]] = true;
                        q.push_back(match[u]);
                    }
                }
            }
        }
        return false;
    };

    for (int i = 0; i < n; ++i) {
        if (match[i] == -1) {
            findAugmentingPath(i);
        }
    }
    return match;
}

