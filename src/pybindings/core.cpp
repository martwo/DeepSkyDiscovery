/**
 * $Id$
 *
 * Copyright (C)
 * 2015 - $Date$
 *     Martin Wolf <deepskydiscovery@martin-wolf.org>
 *
 */
#include <boost/python.hpp>

#include <deepskydiscovery/core.hpp>

namespace bp = boost::python;

namespace deepskydiscovery {

void register_core()
{
    bp::def("geom_img_offset_fine", &py_geom_img_offset_fine
        , ( bp::arg("image_list")
          , bp::arg("offset_estimates")
          , bp::arg("anchors")
          , bp::arg("s_hx")
          , bp::arg("s_hy")
          , bp::arg("m_hx")
          , bp::arg("m_hy")
          )
        , "Determines the offsets of each image relative to the reference image "
          "(i.e. the first image)."
    );
}

}//namespace deepskydiscovery
