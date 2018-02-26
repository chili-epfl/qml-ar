import re

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
pattern = re.compile('connect\(([^,]+), ([A-Z]+)\(([^)]+\))\), ([^,]+), ([A-Z]+)\(([^)]+\))\)')
new_code = []

for i, line in enumerate(open(source, 'r')):
    res = pattern.search(line)
    new_line = line
    if res:
        # functions
        fn1 = res.group(3).split('(')[0]
        fn2 = res.group(6).split('(')[0]
        # objects
        ob1 = res.group(1)
        ob2 = res.group(4)
        # new connect() syntax
        new_connect = 'connect(%s, &%s:%s, %s, &%s:%s' % (ob1, types[ob1], fn1, ob2, types[ob2], fn2)
        new_line = line[:res.start()] + new_connect + line[res.end():]
        print('New connect string: ' + new_connect)
    elif i in connect_lines:
        print('Missed connect: ' + line.strip())
    new_code.append(new_line)

print(''.join(new_code))
