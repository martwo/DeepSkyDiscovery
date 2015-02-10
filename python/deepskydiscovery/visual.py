import numpy as np
import deepskydiscovery as dsd

def plot_image(ax, img):
    import matplotlib.pyplot as plt
    import matplotlib.cm as cm

    ax.imshow(
          img.data.T
        , extent=(0, img.size_x, 0, img.size_y)
        , origin='lower'
        , interpolation='nearest'
        , cmap=cm.gist_rainbow
    )
    #ax.figure.colorbar(ax)
