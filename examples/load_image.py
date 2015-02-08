import numpy as np

import deepskydiscovery as dsd

img = dsd.Image("/home/mwolf/projects/deepsky/moon/DSC03173.fits", dsd.dtype.INT)

#mask = dsd.Mask("/home/mwolf/projects/deepsky/moon/DSC03173.fits")
print("size_x: %d"%img.size_x)
print("size_y: %d"%img.size_y)
print(img.data.shape)
print(img.data)
img.reject(1,1)
print(np.any(img.bpm))
print(img.bpm)
img.shift(+150, +100)
print(img.data)
print(np.mean(img.data))
