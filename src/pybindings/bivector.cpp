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

#include <deepskydiscovery/bivector.hpp>

namespace bp = boost::python;
namespace bn = boost::numpy;

namespace deepskydiscovery {

void register_BiVector()
{
    bp::class_<BiVector, boost::shared_ptr<BiVector> >("BiVector",
        bp::init<
            size_t
        >((bp::arg("n"))
        , "Creates a BiVector of size ``n``."
        )
    )
    .add_property("size", &BiVector::GetSize
        , "The size of the BiVector.")
    .add_property("x", bp::make_function(
          &BiVector::GetX
        , bp::return_internal_reference<>())
        , "The Vector object holding the x-values of the BiVector object.")
    .add_property("y", bp::make_function(
          &BiVector::GetY
        , bp::return_internal_reference<>())
        , "The Vector object holding the y-values of the BiVector object.")
    ;
}

}//namespace deepskydiscovery
