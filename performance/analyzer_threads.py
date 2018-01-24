import sys
import re
import numpy as np
import matplotlib.pyplot as plt

# need one argument as input file
if len(sys.argv) != 2:
    print('Usage: %s input_log_file.txt' % sys.argv[0])
    sys.exit(1)

# opening the file
filename = sys.argv[1]
try:
    f = open(filename, 'r')
except:
    print('Please specify a valid file')
    exit(1)

# regular expression to match
pattern = '^([0-9]+)\s\+([0-9]+)\sms\s([0-9a-z]+)\s\[([^@]+)\@([^:]+):([^]]+)]\s(.*)$'
program = re.compile(pattern)
# 1 timestamp, 2 delta_ms, 3 thread, 4 function, 5 file, 6 line, 7 message

# transition array
execution_time = {}

# ignoring first lines
ignore_lines = 0

# begin time for a task
block_begin = {}

# pattern of messages to analyze
analyzer_pattern = re.compile("^(\[ANALYZE]) ([^ ]+) (.*)$")

# all tids
all_threads = set()

for i, line in enumerate(f):
    # ignoring first lines
    if i < ignore_lines: continue

    # removing newline
    line = line.strip()

    # trying to match with re
    matching = program.split(line)

    # 9 is a magic number (see definition of pattern)
    if matching and len(matching) == 9:
        # parsing the array
        _, timestamp, delta_ms, thread, function, file_, line, message, _ = matching

        # splitting over analyze pattern
        split = analyzer_pattern.split(message)

        if split and len(split) == 5:
            _, _, status, block, _ = split
 #           print(timestamp, thread, block, status)
            if status == 'Begin':
                block_begin[block] = (timestamp, thread)
            elif status == 'End':
                assert(block in block_begin.keys())
                if thread != block_begin[block][1]:
                    print('Multistart for block ' + block + ' tid1 ' + block_begin[block][1] + ' tid2 ' + thread)
                    continue
                if block not in execution_time.keys(): execution_time[block] = []
                execution_time[block].append(int(timestamp) - int(block_begin[block][0]))
                all_threads.add(thread)
            else:
                assert(False)

# printing all successors
print(execution_time)

# all tids
print(all_threads)

# all blocks
print(block_begin.keys())

# printing the result
all_arrays = []
all_labels = []
for block, arr in execution_time.items():
    print('%s, Med %.2f Mean %.2f +- %.2f' % (block, np.median(arr), np.mean(arr), np.std(arr)))
    all_arrays.append(arr)
    all_labels.append('%s: med %.2f/mean %.2f/std %.2f' % (block, np.median(arr), np.mean(arr), np.std(arr)))

plt.figure(figsize=(10, 10))
plt.title('$\Delta_{t}$ for thread loop')
plt.xlabel('ms')
plt.yticks(rotation=25)
plt.boxplot(all_arrays, labels = all_labels, vert = False)
plt.savefig(filename + '.png', bbox_inches='tight')
print('Output: ' + filename + '.png')
