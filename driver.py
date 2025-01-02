import datetime
import subprocess
import random
import networkx as nx
import matplotlib.pyplot as plt
import os

def main():
    num_vertices = random.randint(5, 20)
    num_edges = random.randint(1, num_vertices * (num_vertices - 1) // 2)
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
