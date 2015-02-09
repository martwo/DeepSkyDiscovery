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

#include <deepskydiscovery/vector.hpp>

namespace bp = boost::python;
namespace bn = boost::numpy;

namespace deepskydiscovery {

void register_Vector()
{
    bp::class_<Vector, boost::shared_ptr<Vector> >("Vector",
        bp::init<
            size_t
        >((bp::arg("n"))
        , "Creates a Vector of size ``n``."
        )
    )
    .add_property("owns_cpl_object", &Vector::GetOwnsCPLObject
        , "Flag if this Vector object owns the handled cpl_vector object.")
    .add_property("size", &Vector::GetSize
        , "The size of the Vector.")
    .add_property("data", bp::make_function(
          &Vector::py_get_ndarray
        , bn::ndarray_accessor_return())
        , "The ndarray holding the data values of the Vector object.")
    .def("fill", &Vector::Fill
        , (bp::arg("self"), bp::arg("value"))
        , "Fills the entire vector with the given value.")
    ;
}

}//namespace deepskydiscovery
