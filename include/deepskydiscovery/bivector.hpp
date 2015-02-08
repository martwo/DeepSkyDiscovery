#ifndef DEEPSKYDISCOVERY_BIVECTOR_HPP_INCLUDED
#define DEEPSKYDISCOVERY_BIVECTOR_HPP_INCLUDED 1

#include <string>
#include <sstream>

#include <boost/numpy.hpp>

#include <cpl.h>

#include <deepskydiscovery/error.hpp>

namespace bn = boost::numpy;

namespace deepskydiscovery {

class BiVector
{
  public:
    /** Creates a new BiVector of size n.
     */
    BiVector(size_t n)
    {
        cpl_bivector_ = cpl_bivector_new(n);
        if(! cpl_bivector_)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not create BiVector of size "<<n<<": "
               << msg;
            throw RuntimeError(ss.str());
        }
    }

    /* Creates a BiVector which handles a cpl_bivector resource.
     * Note: This means, that this BiVector object owns the data!
     */
    BiVector(cpl_bivector * vec)
      : cpl_bivector_(vec)
    {}

    virtual
    ~BiVector()
    {
        if(cpl_bivector_)
        {
            cpl_bivector_delete(cpl_bivector_);
        }
    }

    size_t
    GetSize() const
    {
        return cpl_bivector_get_size(cpl_bivector_);
    }

    bn::ndarray
    py_get_x_ndarray() const
    {
        bn::dtype dt = bn::dtype::get_builtin<double>();
        std::vector<intptr_t> shape(1);
        shape[0] = GetSize();
        std::vector<intptr_t> strides(1);
        strides[0] = dt.get_itemsize();
        void * data = cpl_bivector_get_x_data(cpl_bivector_);
        return bn::from_data(data, dt, shape, strides, NULL);
    }

    bn::ndarray
    py_get_y_ndarray() const
    {
        bn::dtype dt = bn::dtype::get_builtin<double>();
        std::vector<intptr_t> shape(1);
        shape[0] = GetSize();
        std::vector<intptr_t> strides(1);
        strides[0] = dt.get_itemsize();
        void * data = cpl_bivector_get_y_data(cpl_bivector_);
        return bn::from_data(data, dt, shape, strides, NULL);
    }

  protected:
    cpl_bivector * cpl_bivector_;
};

}//namespace deepskydiscovery

#endif // !DEEPSKYDISCOVERY_BIVECTOR_HPP_INCLUDED
