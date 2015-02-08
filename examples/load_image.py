import numpy as np

import deepskydiscovery as dsd

img = dsd.Image("/home/mwolf/projects/deepsky/moon/DSC03173.fits", dsd.dtype.INT)
print("size_x: %d"%img.size_x)
print("size_y: %d"%img.size_y)
data = img.data
print(data.shape)
print(data)
print(np.mean(data))
