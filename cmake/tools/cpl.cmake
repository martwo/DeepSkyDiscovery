#
# $Id$
#
# Copyright (C)
# 2014 - $Date$
#     Martin Wolf <ndhist@martin-wolf.org>
#
# This file implements the cmake find_boostnumpy function searching for
# the BoostNumpy tool using the find_tool function.
#
# This file is distributed under the BSD 2-Clause Open Source License
# (See LICENSE file).
#
function(find_cpl)

    find_tool(cpl
        include
        cpl.h
        lib
        cext cfitsio cplcore cpldfs cpldrs cplui fftw3 wcs
    )

endfunction(find_cpl)
