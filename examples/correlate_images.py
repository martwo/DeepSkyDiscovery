import deepskydiscovery as dsd

imglist = [
    dsd.Image("/home/mwolf/projects/deepsky/moon/DSC03173.fits", dsd.dtype.FLOAT)
  , dsd.Image("/home/mwolf/projects/deepsky/moon/DSC03177.fits", dsd.dtype.FLOAT)
]

offset_estimates = dsd.BiVector(2)
offset_estimates.x.fill(0)
offset_estimates.y.fill(0)

anchors = dsd.BiVector(1)
anchors.x.fill(2750)
anchors.y.fill(3800)

print(imglist[0].get_fwhm(2750, 3800))
#print(imglist[0].iqe(10, 10, 100, 10))
s_hx = 80
s_hy = 80
m_hx = 20
m_hy = 20
(offsets, correl) = dsd.geom_img_offset_fine(imglist, offset_estimates, anchors, s_hx, s_hy, m_hx, m_hy)
print(offsets.x.data)
print(offsets.y.data)
print(correl.data)
