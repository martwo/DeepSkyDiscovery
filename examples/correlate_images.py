import numpy as np
import deepskydiscovery as dsd
import matplotlib as mpl
import matplotlib.pyplot as plt

imglist = [
    dsd.Image("/home/mwolf/projects/deepsky/moon/DSC03215.fits", dsd.dtype.FLOAT)
  , dsd.Image("/home/mwolf/projects/deepsky/moon/DSC03216.fits", dsd.dtype.FLOAT)
]

img0 = imglist[0]

print("pixel value at (4017,2158) from img.get : %f"%imglist[0].get(4017,2158))
print("pixel value at (4017,2158) from img.data: %f"%imglist[0].data[4017,2158])
fig = mpl.figure.Figure()
ax = fig.add_subplot(1,1,1)
dsd.visual.plot_image(ax, imglist[0])
plt.show()

print("Mean 0: %g"%np.mean(imglist[0].data))
print("Mean 1: %g"%np.mean(imglist[1].data))

offset_estimates = dsd.BiVector(2)
offset_estimates.x.fill(0)
offset_estimates.y.fill(0)
#offset_estimates.x.data[1] = 100
#offset_estimates.y.data[1] = -100

anchors = dsd.BiVector(1)
anchors.x.data[0] = 4017
anchors.y.data[0] = 2158
#anchors.x.data[1] = 3800
#anchors.y.data[1] = imglist[0].size_y-2900
#anchors.x.data[2] = 3650
#anchors.y.data[2] = imglist[0].size_y-2730
#anchors.x.data[3] = 3900
#anchors.y.data[3] = imglist[0].size_y-2820

print(imglist[0].get_fwhm(4017, 2158))
fwhm = imglist[0].get_fwhm(4017, 2158)

print("pixel value: %f"%img0.data[4017, 2158])
#print(imglist[0].iqe(4017, 2158, 50, 50))
s_hx = 500
s_hy = 500
m_hx = 70#int(fwhm[0])
m_hy = 70#int(fwhm[1])
(offsets, correl) = dsd.geom_img_offset_fine(imglist, offset_estimates, anchors, s_hx, s_hy, m_hx, m_hy)
print(offsets.x.data)
print(offsets.y.data)
print(correl.data)
