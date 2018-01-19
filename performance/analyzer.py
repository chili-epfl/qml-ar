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
pattern = '^([0-9]+)\s\+([0-9]+)\sms\s\[([^@]+)\@([^:]+):([^]]+)]\s(.*)$'
program = re.compile(pattern)
# 1 timestamp, 2 delta_ms, 3 function, 4 file, 5 line, 6 message

# next item for current item
successors = {}

# transition array
transitions = {}

# ignoring first lines
ignore_lines = 100

# reading lines in the file
prev_message = ''
for i, line in enumerate(f):
    # ignoring first lines
    if i < ignore_lines: continue

    # removing newline
    line = line.strip()

    # trying to match with re
    matching = program.split(line)

    # 8 is a magic number (see definition of pattern)
    if matching and len(matching) == 8:
        # parsing the array
        _, timestamp, delta_ms, function, file_, line, message, _ = matching

        # filling in the successor
        if prev_message not in successors.keys():
            successors[prev_message] = set()
        successors[prev_message].add(message)

        # filling in the transition
        transition = (prev_message, message)
        if transition not in transitions.keys():
            transitions[transition] = []
        transitions[transition].append(float(delta_ms))

        # updating previous message
        prev_message = message

# printing all successors
#print(successors)

# printing oversized items
s_length = [(k, len(v)) for k, v in successors.items() if len(v) > 1]
for s, l in s_length:
    print('Oversized %s (%d): %s' % (s, l, successors[s]))

# sorting transition array by median
transition_arr = sorted(list(transitions.items()), key = lambda x : np.median(x[1]), reverse = True)

# printing the result
all_arrays = []
all_labels = []
for (source, dest), arr in transition_arr:
    print('%s->%s, Med %.2f Mean %.2f +- %.2f' % (source, dest, np.median(arr), np.mean(arr), np.std(arr)))
    all_arrays.append(arr)
    all_labels.append('%s to %s' % (source, dest))

plt.figure(figsize=(10, 10))
plt.title('$\delta_{ms}$ for transitions')
plt.yticks(rotation=25)
plt.boxplot(all_arrays, labels = all_labels, vert = False)
plt.savefig(filename + '.png', bbox_inches='tight')
