#ifndef DEEPSKYDISCOVERY_IMAGE_HPP_INCLUDED
#define DEEPSKYDISCOVERY_IMAGE_HPP_INCLUDED 1

#include <iostream>

#include <boost/python.hpp>

#include <boost/numpy.hpp>

// Note: CPL includes complex.h which defines the pre-processor macro I for the
//       imaginary complex number.
//       This clashes with BoostNumpy when defining a static constant class
//       member named I via BOOST_STATIC_CONSTANT.
//       So we need to include the CPL library last.
#include <cpl.h>

namespace bp = boost::python;
namespace bn = boost::numpy;

namespace deepskydiscovery {

namespace image {

enum DType {
    DTYPE_INT    = CPL_TYPE_INT,
    DTYPE_FLOAT  = CPL_TYPE_FLOAT,
    DTYPE_DOUBLE = CPL_TYPE_DOUBLE
};

}//namespace image

class Image
{
  public:
    Image(
        std::string const & pathfilename
      , image::DType dtype
      , size_t plane_idx=0
      , size_t ext_idx=0
    )
      : pathfilename_(pathfilename)
      , dtype_(dtype)
      , plane_idx_(plane_idx)
      , ext_idx_(ext_idx)
    {
        image_ = cpl_image_load(pathfilename_.c_str(), (cpl_type)dtype_, plane_idx_, ext_idx_);

        if(! image_)
        {
            std::cout << "Error loading the image file '"<<pathfilename_<<"'"<<std::endl;
            return;
        }
    }

    virtual
    ~Image()
    {
        if(image_) {
            cpl_image_delete(image_);
        }
    }

    /** Gets the pixel value at the specified position. If it is a bad pixel, or
     *  an error occurred, NAN is returned.
     */
    double
    Get(size_t xpos, size_t ypos) const
    {
        int errcode;
        double value = cpl_image_get(image_, xpos, ypos, &errcode);
        if(errcode != 0)
        {
            return NAN;
        }
        return value;
    }

    size_t
    GetSizeX() const
    {
        return cpl_image_get_size_x(image_);
    }

    size_t
    GetSizeY() const
    {
        return cpl_image_get_size_y(image_);
    }

    bn::ndarray
    py_get_ndarray() const
    {
        bn::dtype dt =
            ( dtype_ == image::DTYPE_INT ? bn::dtype::get_builtin<int>()
            : dtype_ == image::DTYPE_FLOAT ? bn::dtype::get_builtin<float>()
            : bn::dtype::get_builtin<double>());
        std::vector<intptr_t> shape(2);
        shape[0] = GetSizeX();
        shape[1] = GetSizeY();
        std::vector<intptr_t> strides(2);
        strides[1] = dt.get_itemsize();
        strides[0] = shape[1]*strides[1];
        return bn::from_data(cpl_image_get_data(image_), dt, shape, strides, NULL);
    }

    std::string pathfilename_;
    image::DType dtype_;
    size_t const plane_idx_;
    size_t const ext_idx_;
    cpl_image * image_;
};

}//namespace deepskydiscovery

#endif // !DEEPSKYDISCOVERY_IMAGE_HPP_INCLUDED
