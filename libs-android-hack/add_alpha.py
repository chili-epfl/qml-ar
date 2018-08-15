import numpy as np
from scipy.ndimage import imread
from scipy.misc import imsave
import sys

img_fn = sys.argv[1]
out_fn = img_fn + '_alpha.png'
print('Input: %s\nOutput: %s' % (img_fn, out_fn))
img = imread(img_fn)
img[:, :, 3] = 255
imsave(out_fn, img)
