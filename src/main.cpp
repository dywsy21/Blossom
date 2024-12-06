#include "graph_max_matching.h"
#include "getopt.h"
#include <iostream>

using std::cin; 
using std::cout;

int main() {
    cin.tie(0); cout.tie(0); std::ios::sync_with_stdio(0);
    int n, k; cin >> n >> k;
    Graph graph(n);
    for (int i = 0; i < k; ++i) {
        int u, v;
        cin >> u >> v;
        graph.addEdge(u, v);
    }
    GraphSolver solver(graph);
    std::vector<int> matching = solver.findMaximumMatching();
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