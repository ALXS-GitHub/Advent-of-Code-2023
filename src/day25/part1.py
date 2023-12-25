import networkx as nx
import time

start = time.time_ns()

E = []
P = set()
visualize = True  # Set to True to visualize the graph

with open("inputs/day25/input.txt", "r") as f:
    lines = f.readlines()
    for line in lines:
        line = line.strip()

        parts = line.split(":")
        parts[1] = parts[1].strip()
        for x in parts[1].split(" "):
            E.append([parts[0].strip(), x.strip()])
            P.add(parts[0].strip())
            P.add(x.strip())

G = nx.Graph()
for e in E:
    G.add_edge(e[0], e[1], capacity=1)

found = False
for x in P:
    for y in P:
        if x == y:
            continue
        cut_value, partition = nx.minimum_cut(G, x, y)
        if cut_value == 3:
            print(len(partition[0]) * len(partition[1]))
            found = True
            break
    if found:
        break

end = time.time_ns()

elapsed_ns = (end - start) / 1000000000
print(f"Time {elapsed_ns} seconds")

if visualize:
    import matplotlib.pyplot as plt

    pos = nx.spring_layout(G)

    # mbk rrl ddl qnd pcs lcm 
    options = {
        "node_color": "yellow",
        "edge_color": "red",
        "node_size": 360,
        "width": 2,
        "font_size": 10,
        "with_labels": False,
    }
    
    labels = {'mbk': 'mbk', 'rrl': 'rrl', 'ddl': 'ddl', 'qnd': 'qnd', 'pcs': 'pcs', 'lcm': 'lcm'}
    
    nx.draw(G, pos, **options)
    nx.draw_networkx_labels(G, pos=pos, labels=labels)
    
    plt.show()
