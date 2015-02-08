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

#include <deepskydiscovery/image.hpp>

namespace bp = boost::python;
namespace bn = boost::numpy;

namespace deepskydiscovery {

void register_Image()
{
    bp::enum_<image::DType>("dtype")
        .value("INT",    image::DTYPE_INT)
        .value("FLOAT",  image::DTYPE_FLOAT)
        .value("DOUBLE", image::DTYPE_DOUBLE)
    ;

    bp::class_<Image, boost::shared_ptr<Image> >("Image",
        bp::init<
            std::string const &
          , image::DType
          , size_t
          , size_t
        >((bp::arg("pathfilename")
          ,bp::arg("dtype")
          ,bp::arg("plane_idx")=0
          ,bp::arg("ext_idx")=0
          )
        , "Loads the specified image from disc."
        )
    )

    .add_property("size_x", &Image::GetSizeX
        , "The size of the image in the x-direction.")
    .add_property("size_y", &Image::GetSizeY
        , "The size of the image in the y-direction.")

    .add_property("data", bp::make_function(
          &Image::py_get_ndarray
        , bn::ndarray_accessor_return())
        , "The ndarray holding the data of the image.")

    .def("get", &Image::Get
        , "Gets the pixel value at the specified position. If it is a bad "
          "pixel, or an error occurred, NAN is returned.")
    ;
}

}//namespace deepskydiscovery
