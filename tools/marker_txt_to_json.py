#!/usr/bin/env python3
import sys
import json

# need input file to work with
if len(sys.argv) != 2:
    print("Usage: %s input.txt" % sys.argv[0])
    sys.exit(1)

# opening file
f = open(sys.argv[1], 'r')

# reading all lines
lines = [line.strip() for line in f]

# obtaining number of points (first line)
n = int(lines[0])

# array for dots
points = []

# loop over remaining lines
for i in range(1, n + 1):
    # obtaining dot coordinates
    x, y = [int(t) for t in lines[i].split()]

    # saving them
    points.append({"x": x, "y": y})

# resulting json string
res = json.dumps({'dots_uchiya': points}, indent = 2)

# printing json string
print(res)
