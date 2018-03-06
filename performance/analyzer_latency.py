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

# stored_data[index_str] = [int1, int2, ...]
stored_data = {}

for i, line in enumerate(f):
    # removing newline
    line = line.strip()

    # trying to match with re
    matching = program.split(line)

    # 9 is a magic number (see definition of pattern)
    if matching and len(matching) == 9:
        # parsing the array
        _, timestamp, delta_ms, thread, function, file_, line, message, _ = matching

        # looking for the keyword
        if message.find('[ANALYZE] LAT') == 0:
            info_pairs = message[14:].split()
	    info_pairs = zip(info_pairs[::2], info_pairs[1::2])[2:]
            for block, value in info_pairs:
                if block not in stored_data: stored_data[block] = []
                stored_data[block].append(int(value))

# printing all info
print(stored_data)

# all blocks
print('Blocks:')
print(list(stored_data.keys()))

# printing the result
all_arrays = []
all_labels = []
all_blocks = []
for block, arr in stored_data.items():
    if len(arr) > 0:
        all_arrays.append(arr)
        all_labels.append('%s $\\rightarrow$ latency\n $f_{single}$ $%.1f$, ms: $%.2f\pm %.2f$ med $%.2f$' % (block,  1000./ np.mean(arr), np.mean(arr), np.std(arr), np.median(arr)))
        all_blocks.append(block)

for label in all_labels:
    print(' '.join(label.split()))

# plot bar chart
plt.figure(figsize=(15, 15))
plt.title('Processing time for block (input identity preserved)')
plt.xlabel('ms')
#plt.yticks(rotation=25)
plt.boxplot(all_arrays, labels = all_labels, vert = False)
plt.savefig(filename + '_lat.png', bbox_inches='tight')
print('Output: ' + filename + '_lat.png')
plt.clf()

# plot historgrams
for arr, block in zip(all_arrays, all_blocks):
    plt.figure(figsize=(10, 10))
    plt.hist(arr, 20, facecolor='g', alpha=0.75)
    plt.xlabel('ms')
    plt.ylabel('Count')
    plt.title('Histogram of ' + block + ' (lat) time')
    plt.grid(True)
    fn_local = filename + '_' + block + '_lat.png'
    plt.savefig(fn_local, bbox_inches='tight')
    print('Block ' + block + ': ' + fn_local)
    plt.clf()

