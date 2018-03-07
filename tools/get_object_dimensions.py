import sys
from matplotlib import pyplot as plt
import numpy as np
import itertools

# input obj file
try:
    fn = sys.argv[1]
except:
    print('Usage: %s model.obj' % sys.argv[0])
    sys.exit(1)

# object vertices
points = []

# reading vertices
with open(sys.argv[1], 'r') as f:
    for line in f:
        line = line.strip()
        if line[:2] != 'v ': continue
        _, x, y, z = line.split()
        x, y, z = map(float, [x, y, z])
        points.append([x, y, z])

# to numpy
points = np.array(points)

# axes names
axes = [0, 1, 2]
axes_names = 'xyz'

# all 2-subsets of axes
for a1, a2 in itertools.combinations(axes, 2):
    name = ''.join([axes_names[a] for a in [a1, a2]])
    print('Plotting ' + name)
    plt.figure()
    plt.scatter(points[:, a1], points[:, a2])
    plt.savefig(fn + '-' + name + '.png', bbox_inches = 'tight')
