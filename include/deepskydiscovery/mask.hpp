#ifndef DEEPSKYDISCOVERY_MASK_HPP_INCLUDED
#define DEEPSKYDISCOVERY_MASK_HPP_INCLUDED 1

#include <boost/numpy.hpp>

#include <cpl.h>

#include <deepskydiscovery/error.hpp>

namespace bn = boost::numpy;

namespace deepskydiscovery {

class Mask
{
  public:
    // Loads the mask from a FITS image.
    Mask(
        std::string const & pathfilename
      , size_t plane_idx=0
      , size_t ext_idx=0
    )
    {
        cpl_mask_ = cpl_mask_load(pathfilename.c_str(), plane_idx, ext_idx);
        if(! cpl_mask_)
        {
            std::string msg(cpl_error_get_message());
            if(cpl_error_get_code() == CPL_ERROR_DATA_NOT_FOUND)
            {
                std::stringstream ss;
                ss << "The specified file \""<<pathfilename<<"\" does "
                   << "not contain mask data!";
                throw ValueError(ss.str());
            }
            else
            {
                std::stringstream ss;
                ss << "The mask could not be loaded from the file "
                   << "\""<<pathfilename<<"\": "<<msg;
                throw RuntimeError(ss.str());
            }
        }
    }

    // Copy-Constructor.
    Mask(Mask const & m)
    {
        cpl_mask_ = cpl_mask_duplicate(m.cpl_mask_);
        if(! cpl_mask_)
        {
            throw RuntimeError("The mask could not be duplicated!");
        }
    }

    virtual
    ~Mask()
    {
        if(cpl_mask_)
        {
            cpl_mask_delete(cpl_mask_);
        }
    }

    size_t
    GetSizeX() const
    {
        return cpl_mask_get_size_x(cpl_mask_);
    }

    size_t
    GetSizeY() const
    {
        return cpl_mask_get_size_y(cpl_mask_);
    }

    bn::ndarray
    py_get_ndarray() const
    {
        bn::dtype dt = bn::dtype::get_builtin<bool>();
        std::vector<intptr_t> shape(2);
        shape[0] = GetSizeX();
        shape[1] = GetSizeY();
        std::vector<intptr_t> strides(2);
        strides[1] = dt.get_itemsize();
        strides[0] = shape[1]*strides[1];
        return bn::from_data(cpl_mask_get_data(cpl_mask_), dt, shape, strides, NULL);
    }

  protected:
    cpl_mask * cpl_mask_;
};

}//namespace deepskydiscovery

#endif // !DEEPSKYDISCOVERY_MASK_HPP_INCLUDED
