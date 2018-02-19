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

# begin deltas
begin_times = {}

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
                if block not in begin_times: begin_times[block] = []
                begin_times[block].append(int(timestamp))
            elif status == 'End':
                if block not in block_begin.keys():
                    print('Stray block end: %s %s %s' % (block, timestamp, thread))
                    continue
                if thread != block_begin[block][1]:
                    print('Multistart for block ' + block + ' tid1 ' + block_begin[block][1] + ' tid2 ' + thread)
                    continue
                if block not in execution_time.keys(): execution_time[block] = []
                execution_time[block].append(int(timestamp) - int(block_begin[block][0]))
                all_threads.add(thread)
            else:
                assert(False)

# begin deltas for blocks
begin_deltas = {x: list(filter(lambda x: x < 1000, list(np.array(y[1:]) - np.array(y[:-1])))) for x, y in begin_times.items()}

# printing all successors
#print(execution_time)

# all tids
print('Used threads:')
print(all_threads)

# all blocks
print('Blocks:')
print(block_begin.keys())

# show only these blocks
#block_filter = {'Uchiya': 'Detector', 'QtCameraMain': 'Camera', 'HueThresholdManual': 'HSV threshold', 'FrameConvert': 'yuv2rgb'}
block_filter = {x: x for x in block_begin.keys()}

# printing the result
all_arrays = []
all_labels = []
all_blocks = []
for block, arr in execution_time.items():
    if block not in block_filter.keys(): continue
    block = block_filter[block]
    all_arrays.append(arr)
    all_labels.append('%s $\\rightarrow$\n$%.1f$ FPS, ms: $%.2f\pm %.2f$ med $%.2f$' % (block,  1000./ np.mean(arr), np.mean(arr), np.std(arr), np.median(arr)))
    all_blocks.append(block)
    arr1 = begin_deltas[block]
    all_arrays.append(arr1)
    all_labels.append('%s $\circlearrowright$\n$%.1f$ FPS, ms: $%.2f\pm %.2f$ med $%.2f$' % (block,  1000./ np.mean(arr1), np.mean(arr1), np.std(arr1), np.median(arr1)))
    all_blocks.append(block + " Delta")

for label in all_labels:
    print(' '.join(label.split()))

plt.figure(figsize=(15, 15))
plt.title('Processing time for thread loop')
plt.xlabel('ms')
#plt.yticks(rotation=25)
plt.boxplot(all_arrays, labels = all_labels, vert = False)
plt.savefig(filename + '.png', bbox_inches='tight')
print('Output: ' + filename + '.png')
plt.clf()

for arr, block in zip(all_arrays, all_blocks):
    plt.figure(figsize=(10, 10))
    plt.hist(arr, 20, facecolor='g', alpha=0.75)
    plt.xlabel('ms')
    plt.ylabel('Count')
    plt.title('Histogram of ' + block + ' time')
    plt.grid(True)
    fn_local = filename + '_' + block + '.png'
    plt.savefig(fn_local, bbox_inches='tight')
    print('Block ' + block + ': ' + fn_local)
    plt.clf()
