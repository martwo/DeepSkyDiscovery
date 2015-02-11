/**
 * $Id$
 *
 * Copyright (C)
 * 2015 - $Date$
 *     Martin Wolf <deepskydiscovery@martin-wolf.org>
 *
 */
#include <boost/python.hpp>
#include <boost/python/operators.hpp>

#include <boost/numpy.hpp>
#include <boost/numpy/ndarray_accessor_return.hpp>


#include <deepskydiscovery/image.hpp>

namespace bp = boost::python;
namespace bn = boost::numpy;

namespace deepskydiscovery {

namespace detail {

static
bp::tuple
get_fwhm(Image & self, size_t xpos, size_t ypos)
{
    std::pair<double, double> ret = self.GetFWHM(xpos, ypos);
    bp::tuple t = bp::make_tuple<double, double>(ret.first, ret.second);
    return t;
}

}//namespace detail

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
    .def(bp::init<size_t, size_t, image::DType>(
          ( bp::arg("width")
          , bp::arg("height")
          , bp::arg("dtype"))
        , "Creates an image of a certain width and height of the given data type."))

    .add_property("size_x", &Image::GetSizeX
        , "The size of the image in the x-direction.")
    .add_property("size_y", &Image::GetSizeY
        , "The size of the image in the y-direction.")

    .add_property("bpm", bp::make_function(
          &Image::py_get_bpm_ndarray
        , bn::ndarray_accessor_return())
        , "The ndarray holding the bad pixel mask of the image.")

    .add_property("data", bp::make_function(
          &Image::py_get_ndarray
        , bn::ndarray_accessor_return())
        , &Image::py_set_ndarray
        , "The ndarray holding the data of the image.")

    .def("accept", &Image::Accept
        , (bp::arg("self"), bp::arg("xpos"), bp::arg("ypos"))
        , "Sets the specified pixel as good in the image. "
          "The index of each dimension must be in the interval [0,dim_size), "
          "where dim_size is the size of the particular dimension.")
    .def("empty_like", &Image::EmptyLike
        , (bp::arg("self"))
        , "Creates a new image with the same dimensions and data type as this "
          "image.")
    .def("fill_noise_uniform", &Image::FillNoiseUniform
        , ( bp::arg("self")
          , bp::arg("vmin")
          , bp::arg("vmax")
          )
        , "Fills the image with uniform random noise with pixel values ranging "
          "from ``vmin`` to ``vmax``.")
    .def("get", &Image::Get
        , (bp::arg("self"), bp::arg("xpos"), bp::arg("ypos"))
        , "Gets the pixel value at the specified position. If it is a bad "
          "pixel, or an error occurred, NAN is returned.")
    .def("get_fwhm", &detail::get_fwhm
        , (bp::arg("self"), bp::arg("xpos"), bp::arg("ypos"))
        , "Gets the full-width-at-half-maximum for an object at position "
          "(xpos, ypos).")
    .def("get_min_window", &Image::GetMinWindow
        , (bp::arg("self"), bp::arg("llx"), bp::arg("lly"), bp::arg("urx"), bp::arg("ury"))
        , "Determines the minimal pixel value within the specified image window, "
          "which is specified through the lower left and upper right position.")
    .def("iqe", &Image::IQE
        , (bp::arg("self"), bp::arg("xpos"), bp::arg("ypos"), bp::arg("dx"), bp::arg("dy"))
        , "")
    .def("reject", &Image::Reject
        , (bp::arg("self"), bp::arg("xpos"), bp::arg("ypos"))
        , "Sets the specified pixel as bad in the image. "
          "The index of each dimension must be in the interval [0,dim_size), "
          "where dim_size is the size of the particular dimension.")
    .def("shift", &Image::Shift
        , (bp::arg("self"), bp::arg("dx"), bp::arg("dy"))
        , "Shifts the image by the specified number of pixels (can be "
          "negative). The new zones (in the result image) where no new value "
          "is computed are set to 0 and flagged as bad pixels.")

    .def(bp::self += bp::self)
    .def(bp::self -= bp::self)
    ;
}

}//namespace deepskydiscovery
