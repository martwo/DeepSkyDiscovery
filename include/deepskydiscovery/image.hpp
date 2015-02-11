#ifndef DEEPSKYDISCOVERY_IMAGE_HPP_INCLUDED
#define DEEPSKYDISCOVERY_IMAGE_HPP_INCLUDED 1

#include <string>
#include <sstream>

#include <boost/python.hpp>

#include <boost/numpy.hpp>

// Note: CPL includes complex.h which defines the pre-processor macro I for the
//       imaginary complex number.
//       This clashes with BoostNumpy when defining a static constant class
//       member named I via BOOST_STATIC_CONSTANT.
//       So we need to include the CPL library last.
#include <cpl.h>

#include <deepskydiscovery/error.hpp>
#include <deepskydiscovery/bivector.hpp>

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
      : dtype_(dtype)
      , plane_idx_(plane_idx)
      , ext_idx_(ext_idx)
    {
        cpl_image_ = cpl_image_load(pathfilename.c_str(), (cpl_type)dtype_, plane_idx_, ext_idx_);
        if(! cpl_image_)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "The image file \""<< pathfilename
               <<"\" could not be loaded: "<<msg;
            throw RuntimeError(ss.str());
        }
    }

    Image(
        size_t width
      , size_t height
      , image::DType dtype
    )
      : dtype_(dtype)
      , plane_idx_(0)
      , ext_idx_(0)
    {
        cpl_image_ = cpl_image_new(width, height, (cpl_type)dtype_);
        if(! cpl_image_)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "The image of size "<<width<<"x"<<height<<" "
               << "could not be created: "<<msg;
            throw RuntimeError(ss.str());
        }
    }

    Image(Image const & other)
      : dtype_(other.dtype_)
      , plane_idx_(other.plane_idx_)
      , ext_idx_(other.ext_idx_)
    {
        cpl_image_ = cpl_image_duplicate(other.cpl_image_);
        if(! cpl_image_)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not copy the image :"<<msg;
            throw RuntimeError(ss.str());
        }
    }

    virtual
    ~Image()
    {
        if(cpl_image_) {
            cpl_image_delete(cpl_image_);
        }
    }

    /** Sets the specified pixel as good in the image.
     *  The index of each dimension must be in the interval [0,dim_size), where
     *  dim_size is the size of the particular dimension.
     */
    void
    Accept(size_t xpos, size_t ypos)
    {
        cpl_error_code errcode = cpl_image_accept(cpl_image_, xpos+1, ypos+1);
        if(errcode != CPL_ERROR_NONE)
        {
            if(errcode == CPL_ERROR_ACCESS_OUT_OF_RANGE)
            {
                std::stringstream ss;
                ss << "The pixel position ("<<xpos<<","<<ypos<<") is "
                   << "out-of-range!";
                throw IndexError(ss.str());
            }
            else
            {
                std::string msg(cpl_error_get_message());
                std::stringstream ss;
                ss << "Could not accept pixel ("<<xpos<<","<<ypos<<"): "
                   << msg;
                throw RuntimeError(ss.str());
            }
        }
    }

    /** Creates an empty image with the same dimensions and data type as this
     *  image.
     */
    Image
    EmptyLike()
    {
        return Image(GetSizeX(), GetSizeY(), GetType());
    }

    void
    FillNoiseUniform(double vmin, double vmax)
    {
        cpl_error_code errcode = cpl_image_fill_noise_uniform(cpl_image_, vmin, vmax);
        if(errcode != CPL_ERROR_NONE)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not fill the image with noise uniformly: "<<msg;
            throw RuntimeError(ss.str());
        }
    }

    /** Gets the pixel value at the specified position. If it is a bad pixel, or
     *  an error occurred, NAN is returned.
     */
    double
    Get(size_t xpos, size_t ypos) const
    {
        int pixel_is_rejected;
        double value = cpl_image_get(cpl_image_, xpos+1, ypos+1, &pixel_is_rejected);
        if(cpl_error_get_code() != CPL_ERROR_NONE)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not get value of pixel ("<<xpos<<","<<ypos<<"): "<<msg;
            throw RuntimeError(ss.str());
        }
        if(pixel_is_rejected != 0)
        {
            return NAN;
        }
        return value;
    }

    cpl_image *
    GetCPLImage() const
    {
        return cpl_image_;
    }

    std::pair<double, double>
    GetFWHM(size_t xpos, size_t ypos)
    {
        std::pair<double, double> ret = std::make_pair(0, 0);
        cpl_error_code errcode = cpl_image_get_fwhm(cpl_image_, xpos+1, ypos+1, &ret.first, &ret.second);
        if(errcode != CPL_ERROR_NONE)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not get FWHM of pixel ("<<xpos<<","<<ypos<<"): "<<msg;
            throw RuntimeError(ss.str());
        }
        return ret;
    }

    /** Determines the maximal pixel value contained in the specified sub
     *  window (specified through its lower left and upper right point).
     */
    double
    GetMaxWindow(size_t llx, size_t lly, size_t urx, size_t ury) const
    {
        double value = cpl_image_get_max_window(cpl_image_, llx, lly, urx, ury);
        if(cpl_error_get_code() != CPL_ERROR_NONE)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not determine the maximal pixel value of the image "
               << "window defined by ("<<llx<<","<<lly<<";"<<urx<<","<<ury<<"): "<<msg;
            throw RuntimeError(ss.str());
        }
        return value;
    }

    /** Determines the minimal pixel value contained in the specified sub
     *  window (specified through its lower left and upper right point).
     */
    double
    GetMinWindow(size_t llx, size_t lly, size_t urx, size_t ury) const
    {
        double value = cpl_image_get_min_window(cpl_image_, llx, lly, urx, ury);
        if(cpl_error_get_code() != CPL_ERROR_NONE)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not determine the minimal pixel value of the image "
               << "window defined by ("<<llx<<","<<lly<<";"<<urx<<","<<ury<<"): "<<msg;
            throw RuntimeError(ss.str());
        }
        return value;
    }

    /** Returns the size of the image in the x-dimension.
     */
    size_t
    GetSizeX() const
    {
        return cpl_image_get_size_x(cpl_image_);
    }

    /** Returns the size of the image in the y-dimension.
     */
    size_t
    GetSizeY() const
    {
        return cpl_image_get_size_y(cpl_image_);
    }

    image::DType
    GetType() const
    {
        return image::DType(cpl_image_get_type(cpl_image_));
    }

    BiVector
    IQE(size_t xpos, size_t ypos, size_t dx, size_t dy)
    {
        cpl_bivector * vec = cpl_image_iqe(cpl_image_, xpos, ypos, dx, dy);
        if(! vec)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not compute an image quality estimation for pixel "
               << "("<<xpos<<","<<ypos<<"): "<<msg;
            throw RuntimeError(ss.str());
        }
        BiVector ret(vec);
        return ret;
    }

    /** Sets the specified pixel as bad in the image.
     *  The index of each dimension must be in the interval [0,dim_size), where
     *  dim_size is the size of the particular dimension.
     */
    void
    Reject(size_t xpos, size_t ypos)
    {
        cpl_error_code errcode = cpl_image_reject(cpl_image_, xpos+1, ypos+1);
        if(errcode != CPL_ERROR_NONE)
        {
            if(errcode == CPL_ERROR_ACCESS_OUT_OF_RANGE)
            {
                std::stringstream ss;
                ss << "The pixel position ("<<xpos<<","<<ypos<<") is "
                   << "out-of-range!";
                throw IndexError(ss.str());
            }
            else
            {
                std::string msg(cpl_error_get_message());
                std::stringstream ss;
                ss << "Could not reject pixel ("<<xpos<<","<<ypos<<"): "
                   << msg;
                throw RuntimeError(ss.str());
            }
        }
    }

    /** Shifts the image by the specified number of pixels (can be negative).
     *  The new zones (in the result image) where no new value is computed are
     *  set to 0 and flagged as bad pixels.
     */
    void
    Shift(intptr_t dx, intptr_t dy)
    {
        cpl_error_code errcode = cpl_image_shift(cpl_image_, dx, dy);
        if(errcode != CPL_ERROR_NONE)
        {
            if(errcode == CPL_ERROR_ILLEGAL_INPUT)
            {
                std::stringstream ss;
                ss << "The specified shift values must be in the "
                   << "interval (-SizeX, SizeX)";
                throw IndexError(ss.str());
            }
            else
            {
                throw RuntimeError("Could not shift the image.");
            }
        }
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
        strides[0] = dt.get_itemsize();
        strides[1] = shape[0]*strides[0];
        return bn::from_data(cpl_image_get_data(cpl_image_), dt, shape, strides, NULL);
    }

    void
    py_set_ndarray(bn::ndarray const & arr)
    {
        bn::ndarray old_arr = py_get_ndarray();
        bn::copy_into(old_arr, arr);
    }

    bn::ndarray
    py_get_bpm_ndarray() const
    {
        cpl_mask * mask = cpl_image_get_bpm(cpl_image_);
        bn::dtype dt = bn::dtype::get_builtin<bool>();
        std::vector<intptr_t> shape(2);
        shape[0] = cpl_mask_get_size_x(mask);
        shape[1] = cpl_mask_get_size_y(mask);
        std::vector<intptr_t> strides(2);
        strides[1] = dt.get_itemsize();
        strides[0] = shape[1]*strides[1];
        return bn::from_data(cpl_mask_get_data(mask), dt, shape, strides, NULL);
    }

    Image &
    operator+=(Image const & other)
    {
        cpl_error_code errcode = cpl_image_add(cpl_image_, other.cpl_image_);
        if(errcode != CPL_ERROR_NONE)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not add image to this image: "<<msg;
            throw RuntimeError(ss.str());
        }
        return *this;
    }

    Image &
    operator-=(Image const & other)
    {
        cpl_error_code errcode = cpl_image_subtract(cpl_image_, other.cpl_image_);
        if(errcode != CPL_ERROR_NONE)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not subtract image from this image: "<<msg;
            throw RuntimeError(ss.str());
        }
        return *this;
    }

  protected:
    image::DType dtype_;
    size_t const plane_idx_;
    size_t const ext_idx_;
    cpl_image * cpl_image_;
};

}//namespace deepskydiscovery

#endif // !DEEPSKYDISCOVERY_IMAGE_HPP_INCLUDED
