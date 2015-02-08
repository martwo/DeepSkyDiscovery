/**
 * $Id$
 *
 * Copyright (C)
 * 2015 - $Date$
 *     Martin Wolf <deepskydiscovery@martin-wolf.org>
 *
 */
#include <boost/python.hpp>

#include <boost/numpy.hpp>
#include <boost/numpy/ndarray_accessor_return.hpp>

#include <deepskydiscovery/mask.hpp>

namespace bp = boost::python;
namespace bn = boost::numpy;

namespace deepskydiscovery {

void register_Mask()
{
    bp::class_<Mask, boost::shared_ptr<Mask> >("Mask",
        bp::init<
            std::string const &
          , size_t
          , size_t
        >((bp::arg("pathfilename")
          ,bp::arg("plane_idx")=0
          ,bp::arg("ext_idx")=0
          )
        , "Loads a mask from the specified image on disc."
        )
    )

    .add_property("data", bp::make_function(
          &Mask::py_get_ndarray
        , bn::ndarray_accessor_return())
        , "The ndarray holding the data of the mask.")
    ;
}

}//namespace deepskydiscovery
