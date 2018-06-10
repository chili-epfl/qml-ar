import json

# after joining json files by hand, the numbering is not consistent: 0, 1, ..., N1-1, 0, 1, ..., N2-1
# This script makes it 0, 1, ..., N1+N2-1

inp = json.load(open('markers.json', 'r'))

idx = 0
for marker in inp['markers']:
    print marker['id']
    marker['id'] = idx
    idx += 1

json.dump(inp, open('markers.json', 'w'), indent = 2)
