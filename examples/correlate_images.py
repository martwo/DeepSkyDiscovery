import numpy as np
import deepskydiscovery as dsd
import matplotlib as mpl
import matplotlib.pyplot as plt

imglist = [
    dsd.Image("/home/mwolf/projects/deepsky/moon/DSC03215.fits", dsd.dtype.FLOAT, 2)
  , dsd.Image("/home/mwolf/projects/deepsky/moon/DSC03216.fits", dsd.dtype.FLOAT, 2)
]

img0 = imglist[0]

print("pixel value at (4017,2158) from img.get : %f"%imglist[0].get(4017,2158))
print("pixel value at (4017,2158) from img.data: %f"%imglist[0].data[4017,2158])



print("Mean 0: %g"%np.mean(imglist[0].data))
print("Mean 1: %g"%np.mean(imglist[1].data))

offset_estimates = dsd.BiVector(2)
offset_estimates.x.fill(0)
offset_estimates.y.fill(0)

anchors = dsd.BiVector(1)
anchors.x.data[0] = 4017
anchors.y.data[0] = 2158

print(imglist[0].get_fwhm(4017, 2158))
fwhm = imglist[0].get_fwhm(4017, 2158)

bg_img = imglist[0].empty_like()
noise_min = np.min(img0.data[2800:3200,2800:3200])
noise_max = np.max(img0.data[2800:3200,2800:3200])
print("noise_min = %g, noise_max = %g"%(noise_min, noise_max))
bg_img.fill_noise_uniform(float(noise_min), float(noise_max))
#dsd.visual.show_image(bg_img, title="Background")



print("pixel value: %f"%img0.data[4017, 2158])
s_hx = 500
s_hy = 500
m_hx = 70
m_hy = 70
(offsets, correl) = dsd.geom_img_offset_fine(imglist, offset_estimates, anchors, s_hx, s_hy, m_hx, m_hy)
print(offsets.x.data)
print(offsets.y.data)
print(correl.data)



imglist[1].shift(int(np.round(offsets.x.data[1])), int(np.round(offsets.y.data[1])))

img = imglist[0].empty_like()
img += imglist[0]
img += imglist[1]

dsd.visual.show_image(img, "w/o background subtraction")

imglist[0] -= bg_img
imglist[1] -= bg_img

imglist[0] += imglist[1]

dsd.visual.show_image(imglist[0], "with background subtraction")

plt.show()
