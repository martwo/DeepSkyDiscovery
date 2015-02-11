import numpy as np
import deepskydiscovery as dsd

def plot_image(ax, img, cmap=None):
    import matplotlib.pyplot as plt
    import matplotlib.cm as cm

    if(cmap is None):
        cmap = cm.gist_heat
    img = ax.imshow(
          img.data.T
        , extent=(0, img.size_x, 0, img.size_y)
        , origin='lower'
        , interpolation='nearest'
        , cmap=cmap
    )
    ax.figure.colorbar(img, ax=ax)

def show_image(img, title=""):
    import matplotlib.pyplot as plt

    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    plot_image(ax, img)
    ax.set_title(title)

