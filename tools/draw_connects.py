import re
import networkx as nx
import matplotlib.pyplot as plt

# input files
source = 'src/qmlar.cpp'
header = 'src/qmlar.h'

# obtaining types
types = {'this': 'QMLAR'}
pattern = re.compile('([^*]+)\* ([^;]+);')
for line in open(header, 'r'):
    res = pattern.search(line)
    if not res: continue
    types[res.group(2).strip()] = res.group(1).strip()

# counting connects
connect_lines = []

for i, line in enumerate(open(source, 'r')):
    if line.find('connect(') >= 0: connect_lines.append(i)

# finding connects and replacing them
pattern = re.compile('[ ]+connect\(([^,]+), &([^:]+)::([^,]+), ([^,]+), &([^:]+)::([a-zA-Z0-9_]+)')
new_code = []

G = nx.Graph()

for i, line in enumerate(open(source, 'r')):
    res = pattern.search(line)
    if res:
        # functions
        fn1 = res.group(3)
        fn2 = res.group(6)
        # objects
        ob1 = res.group(1)
        ob2 = res.group(4)
        # types
        t1 = res.group(2)
        t2 = res.group(5)
        G.add_node(types[ob1])
        G.add_node(types[ob2])
        G.add_edge(types[ob1], types[ob2], weight = '123')
        # new connect() syntax
        #new_connect = 'connect(%s, &%s::%s, %s, &%s::%s)' % (ob1, types[ob1], fn1, ob2, types[ob2], fn2)
        print(types[ob1], fn1, types[ob2], fn2)

# drawing connect() graph
plt.figure(figsize=(15, 15))
plt.axis('off')
nx.draw_networkx(G, with_labels = True)
plt.savefig("path.png", bbox_inches = 'tight')
