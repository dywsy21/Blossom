#include "graph_max_matching.h"
#include "getopt.h"
#include "unionfind.h"
#include <iostream>
#include <unordered_map>
#include <thread>
#include <fstream>

using std::cin; 
using std::cout;

int main(int argc, char* argv[]) {
    bool verbose = false;
    std::string outFileName;
    // This will store the input filename if provided
    std::string inputFile;

    // Parse all dash-starting options first
    int c;
    while ((c = getopt(argc, argv, "hvo:")) != -1) {  // Removed colon after v
        switch(c) {
        case 'h':
            cout << "Usage: program [options] [filename]\n"
                 << "Options:\n"
                 << " -h            Show help\n"
                 << " -v            Verbose mode\n"
                 << " -o outfile    Specify output file\n"
                 << " filename      Input file containing the graph\n";
            return 0;
        case 'v':
            verbose = true;
            break;
        case 'o':
            outFileName = optarg;
            break;
        case '?':
            return 1;
        }
    }

    if (verbose) {
        cout << "Starting graph matching solver...\n";
    }

    // If there is a leftover argument (not starting with '-'),
    // treat it as the input file
    if (optind < argc) {
        inputFile = argv[optind++];
    }

    int n, k;
    UnionFind uf;
    std::vector<std::pair<int,int>> edges;
    Graph graph;

    if (!inputFile.empty()) {
        if (verbose) cout << "Reading from file: " << inputFile << "\n";
        std::ifstream inFile(inputFile);
        if (!inFile) {
            cout << "Error: cannot open file " << inputFile << "\n";
            return 1;
        }
        inFile >> n >> k;
        graph.setSize(n);
        uf.setSize(n);
        // ...existing code to read edges from inFile...
        for (int i = 0; i < k; ++i) {
            int u, v;
            inFile >> u >> v;
            edges.push_back({u, v});
            uf.unite(u, v);
        }
    } else {
        if (verbose) cout << "Reading from standard input...\n";
        // No leftover argument => read from cin
        cin >> n >> k;
        graph.setSize(n);
        uf.setSize(n);
        // ...existing code to read edges from cin...
        for (int i = 0; i < k; ++i) {
            int u, v;
            cin >> u >> v;
            edges.push_back({u, v});
            uf.unite(u, v);
        }
    }

    if (verbose) {
        cout << "Graph loaded: " << n << " vertices, " << k << " edges\n";
        cout << "Finding connected components...\n";
    }

    std::unordered_map<int, std::vector<int>> components;
    for (int i = 0; i < n; ++i) {
        components[uf.findRoot(i)].push_back(i);
    }

    if (verbose) {
        cout << "Found " << components.size() << " connected components\n";
        cout << "Starting parallel matching computation...\n";
    }

    std::vector<int> matching(n, -1);
    std::vector<std::thread> threads;
    std::vector<std::vector<int>> subMatches(components.size());
    int compIndex = 0;

    for (auto &comp : components) {
        threads.push_back(std::thread([&, compIndex]() {
            std::vector<int> &nodes = comp.second;
            std::unordered_map<int,int> idx; // old->new index
            for (int i = 0; i < (int)nodes.size(); ++i) {
                idx[nodes[i]] = i;
            }
            Graph subG(nodes.size());
            for (auto &e : edges) {
                if (uf.findRoot(e.first) == comp.first && uf.findRoot(e.second) == comp.first) {
                    subG.addEdge(idx[e.first], idx[e.second]);
                }
            }

            GraphSolver solver(subG);
            subMatches[compIndex] = solver.findMaximumMatching();
        }));
        compIndex++;
    }

    if (verbose) {
        cout << "Waiting for all threads to complete...\n";
    }

    for (auto &t : threads) t.join();

    if (verbose) {
        cout << "Merging results from all components...\n";
    }

    // Merge results back
    compIndex = 0;
    for (auto &comp : components) {
        std::vector<int> &nodes = comp.second;
        for (int i = 0; i < (int)nodes.size(); ++i) {
            int sm = subMatches[compIndex][i];
            if (sm != -1) {
                matching[nodes[i]] = nodes[sm];
            }
        }
        compIndex++;
    }

    if (verbose) {
        cout << "Computing final matching size...\n";
    }

    int matchCount = 0;
    for (int i = 0; i < n; ++i) {
        if (matching[i] != -1 && i < matching[i]) {
            matchCount++;
        }
    }

    if (!outFileName.empty()) {
        if (verbose) {
            cout << "Writing results to file: " << outFileName << "\n";
        }
        std::ofstream ofs(outFileName);
        if (!ofs) {
            cout << "Error: cannot open file " << outFileName << "\n";
            return 1;
        }
        ofs << "Maximum Matching Size: " << matchCount << "\n";
        for (int i = 0; i < n; ++i) {
            if (matching[i] != -1 && i < matching[i]) {
                ofs << i << " " << matching[i] << "\n";
            }
        }
    } else {
        if (verbose) {
            cout << "Writing results to standard output:\n";
        }
        cout << "Maximum Matching Size: " << matchCount << "\n";
        for (int i = 0; i < n; ++i) {
            if (matching[i] != -1 && i < matching[i]) {
                cout << i << " " << matching[i] << "\n";
            }
        }
    }

    if (verbose) {
        cout << "Program completed successfully.\n";
    }

    return 0;
}
