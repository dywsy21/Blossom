---
title: "对于一般图的最大匹配算法实现"
author: "王思宇 23300240006"
date: \today
using_title: true
using_table_of_content: true
---

\section*{摘要}
    图的最大匹配问题是图论中的一个经典问题，它的应用非常广泛。本文将基于论文*Paths, Trees, and Flowers*[1]使用C/C++实现一种求解一般图的最大匹配的，复杂度为 $O(|V|^2 * |E|)$ 的带花树算法。

**关键字:** 图论, 最大匹配, 带花树算法

# 对于一般图的最大匹配算法实现

## 问题描述与定义

给定一个图 $G = (V, E)$，求出一个最大的边集$M$，使得$M$是$G$的一个匹配。其中最大代表$M$所含边的数量最多。

## 重要理论基础

### 交错路径与增广路径

1. 非匹配点/非匹配边：如果$G$中一点或一边不在匹配$M$中，则称其为非匹配点或非匹配边。
2. 交错路径：一条交错路径是一个匹配边和非匹配边交替出现，且首尾两点至少有一个是非匹配点的路径。
3. 增广路径：一条交错路径，且路径的起点和终点都是非匹配点。(或开始的边和末尾的边均为非匹配边)

显然，如果存在一条增广路径，那么可以通过将路径上的匹配边和非匹配边交换，从而增加匹配的边数。

### Berge's Theorem

根据这一直觉，Berge[2]提出并证明了一个匹配$M$为最大匹配的充要条件：

**一个匹配$M$为最大匹配的充要条件是：图G中不存在首尾顶点均未匹配的交错路径。**

必要性显然，因为如果存在一条增广路径，那么可以通过增广路径来增加匹配的边数。

下证充分性：

为了证明Berge定理，我们首先需要一个引理。取一个图$G$，设$M$和$M'$是$G$中的两个匹配。令$G'$为$M$和$M'$的对称差的结果图，即$(M - M') \cup (M' - M)$。$G'$将由以下几种连通分量组成：

1. 一个孤立的顶点。
2. 一个边在$M$和$M'$之间交替出现的偶环。
3. 一个边在$M$和$M'$之间交替出现的路径，且端点不同。

$G'$中的每个顶点度数最多为2：所连的两条边一条来自$M$，一条来自$M'$；每个顶点度数小于等于2的图必须由孤立顶点、环和路径组成。此外，$G'$中的每条路径和环必须在$M$和$M'$之间交替出现。为了使环做到这一点，它必须包含相等数量的来自$M$和$M'$的边，因此长度必须为偶数。

因此每个连通分支要么是一条路径，要么是偶环，或者是平凡图(孤立点)。引理得证。

我们通过反证法证明Berge定理的充分性：设$M'$是$G$中比$M$更大的匹配。考虑$D$，即$M$和$M'$的对称差。注意到$D$由路径和偶环组成（如前面的引理所述）。由于$M'$比$M$大，$D$包含一个比$M'$多边的分量。这样的分量是$G$中以$M'$的边开始和结束的路径，因此它是一个增广路径，与假设矛盾。故原命题成立。

### 问题与解决方案
我们的思路就是不断遍历所有非匹配点，从而在图中寻找到所有的增广路径，这样就能找到图的最大匹配。

在二分图中，这种方式是可行的，利用这一思想的著名算法包括匈牙利算法[3]。

然而，对于一般的图，失去了二分图的假设就代表着图中可能含有奇环，而增广路径的方法有且仅有一个问题：碰到奇环时，不能简单地将路径上的匹配边和非匹配边交换，因为这样会破坏匹配的性质。(可能一个点连两条边)

带花树算法就旨在解决这个问题。带花树算法的核心思想是：将图中的所有奇环（"花"）缩成一个点，从而将问题转化为一个二分图的最大匹配问题。

## 核心思路

1. **搜索增广路径**
- 从一个未匹配点出发，使用 BFS/DFS 搜索增广路径。
- 搜索过程中，将点分为两类（层级分割），交替加入“匹配边”和“非匹配边”。
- 如果找到另一未匹配点，则可以形成一条增广路径。

2. **处理奇环（缩环操作）**
- 当在搜索中发现一个奇环（即 BFS/DFS 遇到一个已经访问过的点，并且两个路径的深度相同），则将其缩成一个点，在缩环后的图中继续搜索增广路径。

- 如果在缩环后的图中找到了增广路径，将该路径还原到原图中。

3. **匹配扩展**
找到增广路径后，通过反转路径中的匹配和非匹配边，扩大匹配。
如果没有找到增广路径，算法终止，当前匹配即为最大匹配。

# C/C++代码实现思路

代码在总体上保持面向对象的思路，力求实现的函数在样式上与数学表达式几乎相同。
因此，我们将实现以下两个类：

1. **Graph**：图的类，用于存储图的信息。
2. **GraphSolver**: 包含图的最大匹配求解算法的类，用于实现带花树算法，包装了所有要用到的参数等。

## 输入输出的约定

在具体探讨如何实现这两个数据结构之前，我们先需给出一种有效的图输入输出方式。

因此我们约定，程序接收的输入格式如下：

**输入格式:**
1. 第一行包含两个整数 $n$ 和 $k$，分别表示图的顶点数和边数。
2. 接下来 $k$ 行，每行包含两个整数 $u$ 和 $v$，表示图中的一条边 $(u, v)$。($1 <= u, v <= n$)

**输出格式:**
1. 输出最大匹配的边数。
2. 接下来若干行，每行包含一个整数，表示每条匹配上的边的两端点的编号。

因此，主函数的代码如下：

```cpp
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
```

虽然此处用到了一系列还未定义的类与函数，但是我们可以看到，主函数的代码简洁易懂，只需读入图的信息，调用`GraphSolver`类的函数，然后输出结果即可。下面我们深入探讨此处用到的这些还未定义的类与函数如何实现。

## Graph的数据结构

我们使用邻接表来存储图的信息，这样可以在节省空间的同时方便地遍历图的边。

`Graph` 类的定义如下：

```cpp
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
```

## Graph类的实现

以下是`Graph`类中各函数的具体实现：

```cpp
Graph::Graph(int n) : n(n + 7), adj(n + 7) {}

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
```

函数`addEdge`用于添加一条边，函数`getNeighbors`用于获取一个点的邻居，函数`getNumVertices`用于获取图的顶点数。`Graph`的构造函数中，我们将顶点数加上一个常数，以便在后续的操作中不用担心数组越界，在此处取7。

## GraphSolver的数据结构

我们将`GraphSolver`类定义如下：

```cpp
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
```

其中每个参数与函数的作用如下：

所有参数如下：

1. graph：存储图的信息。
2. n：图的顶点数。
3. match：匹配向量，match[i]表示顶点i匹配的顶点。
4. base：基向量，base[i]表示顶点i所在的花的基。
5. parent：父节点向量，parent[i]表示顶点i的父节点。
6. q：队列，用于BFS。
7. inQueue：标志向量，inQueue[i]表示顶点i是否在队列中。
8. inBlossom：标志向量，inBlossom[i]表示顶点i是否在花中。

所有函数如下：

1. lca：找到两个点的最近公共祖先。
2. markPath：标记路径。
3. findAugmentingPath：寻找增广路径。
4. GraphSolver：构造函数。
5. findMaximumMatching：核心算法：使用带花树算法寻找最大匹配。

## GraphSolver类的实现

以下是`GraphSolver`类中各函数的具体实现。我们先将`GraphSolver`的构造函数以及辅助函数`lca`, `markPath`, `findAugmentingPath`的实现给出：

### `GraphSolver`构造函数

在构造函数中，我们初始化了所有的参数，包括匹配向量、基向量、父节点向量、队列、标志向量等，并将传入的图信息`graph`保存在自身参数`GraphSolver::graph`中。

```cpp
GraphSolver::GraphSolver(const Graph& graph) 
    : graph(graph), n(graph.getNumVertices()), match(n + 7, -1), base(n + 7), parent(n + 7), inQueue(n + 7), inBlossom(n + 7) {}
```

### `lca`函数

`lca`函数寻咋顶点a和b的最近公共祖先。首先从顶点a开始，沿着匹配边和父节点路径向上走，标记所有经过的顶点。然后从顶点b开始，沿着匹配边和父节点路径向上走，直到找到第一个被标记的顶点，即为最近公共祖先。

```cpp
int GraphSolver::lca(int a, int b) {
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
}
```

### `markPath`函数

`markPath`函数用于在找到奇环时标记路径。它会从顶点u开始，沿着匹配边和父节点路径向上走，直到到达基b，并标记路径上的所有顶点。具体实现如下：

```cpp
void GraphSolver::markPath(int u, int b, int child) {
    for (; base[u] != b; u = parent[match[u]]) {
        inBlossom[base[u]] = inBlossom[base[match[u]]] = true;
        parent[u] = child;
        child = match[u];
    }
}
```

### `findAugmentingPath`函数

`findAugmentingPath`函数用于寻找增广路径。它会从顶点root开始，使用BFS搜索增广路径。鉴于该函数为带花树算法的核心，我们将其分为几个部分详细介绍：

1. *初始化*：初始化队列、父节点和基向量，并将根节点加入队列。

```cpp
    inQueue.assign(n, false);
    parent.assign(n, -1);
    for (int i = 0; i < n; ++i) base[i] = i;
    q.clear();
    q.push_back(root);
    inQueue[root] = true;
```

2. *BFS搜索*：使用BFS从根节点开始搜索增广路径。

```cpp
    for (size_t qi = 0; qi < q.size(); ++qi) {
        int v = q[qi];
        for (int u : graph.getNeighbors(v)) {
            if (base[v] == base[u] || match[v] == u) continue;
            ...
```

3. *处理邻居节点*：对于每个邻居节点u，如果u是根节点或已经在路径中，则找到最近公共祖先并标记路径；否则，如果u没有父节点，则更新父节点并继续搜索。

从实现的角度看，我们实际上并不需要真的收缩奇环，只需要在找到奇环时标记路径即可。因此，我们在找到奇环时，将路径上的所有顶点都标记为在花中，然后将花中的所有顶点的基向量更新为当前花的基。

```cpp
            ...
            if (u == root || (match[u] != -1 && parent[match[u]] != -1)) {
                // blossom found, contract it
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
                // u is free, add it to the queue
                parent[u] = v;
                if (match[u] == -1) {
                    // found augmenting path
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
                    // u is matched, add its match to the queue
                    inQueue[match[u]] = true;
                    q.push_back(match[u]);
                }
            }
        }
    }
```

4. *找到增广路径*：如果找到增广路径，则更新匹配并返回true；否则，继续搜索。若搜索完毕仍未找到增广路径，则返回false。

```cpp
    ...
    return false;
}
```

完整的`findAugmentingPath`函数实现如下：

```cpp
bool GraphSolver::findAugmentingPath(int root) {
    inQueue.assign(n, false);
    parent.assign(n, -1);
    for (int i = 0; i < n; ++i) base[i] = i;
    q.clear();
    q.push_back(root);
    inQueue[root] = true;

    for (size_t qi = 0; i < q.size(); ++qi) {
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
}
```

### `findMaximumMatching`函数

在实现了`findAugmentingPath`后，我们距离完整的带花树算法只差一步之遥：`findMaximumMatching`函数将不断搜索增广路径，直到找不到增广路径为止。这样，我们就实现了完整的带花树算法，找到了图的最大匹配。

```cpp
std::vector<int> GraphSolver::findMaximumMatching() {
    for (int i = 0; i < n; ++i) {
        if (match[i] == -1) {
            findAugmentingPath(i);
        }
    }
    return match;
}
```

# 挑战问题的实现

在这一部分，我们将基于带花树算法，设计更快速的并行算法来解决最大匹配问题。

我们注意到一个事实：每个连通分支的最大匹配是独立的。因此，我们可以将图分解为多个连通分支，然后在每个连通分支上独立地求解最大匹配，最后将所有连通分支的最大匹配合并即可。

## 判断连通性——并查集的实现

为了实现并行算法，我们首先需要判断图中的连通分支。我们可以使用并查集来实现这一功能。

我们将并查集的实现放在一个独立的类`UnionFind`中，其定义如下：

```cpp
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
```

具体函数的实现如下：

```cpp
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
```

我们主要实现了以下两个功能：

1. `findRoot`：查找x所在的连通分支的根节点。
2. `unite`：合并两个连通分支。

鉴于并查集本身的性质，我们可以在$O(\alpha(n))$的时间内完成这两个操作，其中$\alpha(n)$是Ackermann函数的反函数，增长极其缓慢。这也意味着我们可以在很短的时间内完成图的连通性判断，为并行算法会有更高的效率打下了基础。

## 并行算法的实现

### 使用哈希表储存每个根节点的连通分支的所有节点

在并行算法中，我们需要将每个连通分支的所有节点储存在一个哈希表中，以便在并行求解最大匹配时能够快速找到每个连通分支的所有节点。

首先，我们在读取图的时候顺便使用`unite`来合并连通分支：

```cpp
    for (int i = 0; i < k; ++i) {
        int u, v;
        cin >> u >> v;
        edges.push_back({u, v});
        uf.unite(u, v);
    }
```

然后，使用`unordered_map`来储存每个根节点的连通分支的所有节点：

```cpp
    std::unordered_map<int, std::vector<int>> components;
    for (int i = 0; i < n; ++i) {
        components[uf.findRoot(i)].push_back(i);
    }
```

### 并行求解最大匹配

并行的核心部分逻辑很容易理解：只需为每个连通分支分配一个`thread`来跑，然后各自创建一个`GraphSolver`对象，在每个连通分支上独立求解最大匹配，最后将所有连通分支的最大匹配合并即可。

将各个连通分支并行求解的核心逻辑如下：

```cpp
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
```

然后，在合并所有连通分支的最大匹配之前，我们需要确保所有线程都已经运行完：

```cpp
    for (auto &t : threads) t.join();
```

最后，我们将所有连通分支的最大匹配合并：

```cpp
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
```

这样就完成了并行算法的实现。

# 额外功能

## 让程序接收命令行参数

为了让程序更加灵活，我们可以让程序接收命令行参数，从而可以更方便地调整程序的行为。

我们使用了`getopt`库来实现这一功能。考虑到windows上没有`getopt`库，我们自己实现了一个简易的`getopt`库，详见src/getopt.h，src/getopt.cpp。

我们将程序的命令行参数定义如下：

```
Usage: graph_max_matching [options] [filename]
Options:
 -h            Show help
 -v            Verbose mode
 -o outfile    Specify output file
 filename      Input file containing the graph
```
 
在命令行参数中直接给出输入文件名可以让程序转从这份文件中读取输入，而不必手动输入；`-h`参数可以显示帮助信息；`-v`参数可以打开详细输出模式；`-o`参数可以指定输出文件，而不必将结果输出到标准输出。

一个使用示例为在根目录下输入：

```bash
./build/graph_max_matching -v -o examples/io_file/output.txt examples/io_file/input.txt
```

## 可视化结果：python代码实现

为了更好地展示算法的结果，我们可以将图画出，并将最大匹配的边在图上标记出来。我们可以使用python的`matplotlib`与`networkx`库来实现这一功能。以下的代码位于根目录的`driver.py`文件中：

```python
import datetime
import subprocess
import random
import networkx as nx
import matplotlib.pyplot as plt
import os

def main():
    num_vertices = random.randint(5, 10)
    num_edges = random.randint(num_vertices - 1, num_vertices * (num_vertices - 1) // 2)
    edges = set()

    while len(edges) < num_edges:
        u = random.randint(0, num_vertices - 1)
        v = random.randint(0, num_vertices - 1)
        if u != v:
            edges.add((u, v))

    input_data = f"{num_vertices} {len(edges)}\n"
    for u, v in edges:
        input_data += f"{u} {v}\n"

    process = subprocess.Popen(
        ['build/graph_max_matching.exe'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = process.communicate(input=input_data)

    matching = []
    lines = stdout.strip().split('\n')
    for line in lines[1:]:
        tokens = line.strip().split()
        if len(tokens) == 2:
            u, v = map(int, tokens)
            matching.append((u, v))

    G = nx.Graph()
    G.add_nodes_from(range(num_vertices))
    G.add_edges_from(edges)

    pos = nx.spring_layout(G)
    plt.figure(figsize=(8, 6))

    nx.draw_networkx_edges(G, pos, edgelist=edges, width=1.0, alpha=0.5)
    nx.draw_networkx_edges(G, pos, edgelist=matching, width=2.0, edge_color='r')
    nx.draw_networkx_nodes(G, pos, node_size=300)
    nx.draw_networkx_labels(G, pos)

    date_time = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
    os.makedirs('output_img', exist_ok=True)
    plt.savefig(f'output_img/{date_time}.png')
    plt.show()

if __name__ == '__main__':
    main()
```

在这段代码中，我们首先生成一个随机图，然后调用编译好的C/C++程序来求解最大匹配；最后，我们使用`networkx`与`matplotlib`库将图画出，并将最大匹配的边标记为红色。最终的结果将保存在`output_img`文件夹中，文件名为日期/时间，以确保每张图的文件名均不同。

## 样例图

以下为两张样例图：
![样例图1](examples/pics/20241206150015.png)
![样例图2](examples/pics/20241206150141.png)
从图中我们可以直观地看到：我们实现的带花树算法得到了正确的最大匹配。

# 程序的使用

## 编译

首先，我们需要编译C/C++代码。在项目的根目录下输入以下命令：

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## 运行

运行有两种方式：

1. 运行 `./build/graph_max_matching.exe` ，手动输入图的信息。
2. 使用驱动脚本：首先在根目录下输入 `pip install -r requirements.txt` 安装python的依赖库，然后运行根目录下的 `driver.py` ( `python driver.py` )，自动随机生成图并求解最大匹配。生成的图示将自动显示出来并将保存在 `output_img` 文件夹中。

我们推荐使用第二种方式，因为这样可以更直观地看到算法的结果，并且使用也更加方便。

# 总结

在本论文中，我们实现了求解任意图的最大匹配的带花树算法。我们首先实现了带花树算法的核心逻辑，然后实现了并行算法，最后实现了一些额外功能，如命令行参数的解析、图的可视化等。

在实现的过程中遇到了一些问题，如如何将带花树算法的一些叙述起来方便但不方便落到实处的细节实施下去、如何将图的信息传递给并行线程等。我们通过谨慎选择数据结构，通过C++标准库的`thread`来实现并行算法，最终成功地解决了这些问题。

在实现的过程中，我也学到了很多知识，如将一个算法用程序实现的普遍步骤、并行算法的设计、命令行参数的解析等。这些知识对今后的学习和工作都有很大的帮助。

在未来，程序还可以被进一步完善，如实现更多的额外功能、优化算法的性能等。这些工作将会使我们的程序更加完善，更加实用。

# 参考文献

[1] Edmonds J. Paths, Trees, and Flowers. Canadian Journal of Mathematics. 1965;17:449-467. doi:10.4153/CJM-1965-045-4

[2] C. Berge, Two theorems in graph theory, Proc. Natl. Acad. Sci. U.S., 43 (1957), 842-4.

[3] Mills-Tettey G A, Stentz A, Dias M B. The dynamic hungarian algorithm for the assignment problem with changing costs[J]. Robotics Institute, Pittsburgh, PA, Tech. Rep. CMU-RI-TR-07-27, 2007.
