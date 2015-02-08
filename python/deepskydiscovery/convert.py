import os

def convert_DNG_to_FITS(dngfile, fitsfile):
    """Converts the specified DNG file into a FITS file by using ImageMagick's
    ``convert`` program.

    """
    os.system('convert dng:%s -define quantum:format=floating-point -depth 16 fits:%s'%(dngfile, fitsfile))
