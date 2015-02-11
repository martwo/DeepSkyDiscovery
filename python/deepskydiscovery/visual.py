import numpy as np
import deepskydiscovery as dsd
import matplotlib.pyplot as plt

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

def show_image(img, title=""):
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    plot_image(ax, img)
    ax.set_title(title)

