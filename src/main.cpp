#include "graph_max_matching.h"
#include "getopt.h"
#include <iostream>

using std::cin; 
using std::cout;

int main() {
    cin.tie(0); cout.tie(0); std::ios::sync_with_stdio(0);
    int n, m;
    cin >> n >> m;
    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph.addEdge(u, v);
    }
    std::vector<int> matching = findMaximumMatching(graph);
    int matchCount = 0;
    for (int i = 0; i < n; ++i) {
        if (matching[i] != -1 && i < matching[i]) {
            matchCount++;
        }
    }
    cout << "Maximum Matching Size: " << matchCount << "\n";
    for (int i = 0; i < n; ++i) {
        if (matching[i] != -1 && i < matching[i]) {
            cout << i << " " << matching[i] << "\n";
        }
    }
    return 0;
}