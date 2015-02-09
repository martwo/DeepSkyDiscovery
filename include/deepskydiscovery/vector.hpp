#ifndef DEEPSKYDISCOVERY_VECTOR_HPP_INCLUDED
#define DEEPSKYDISCOVERY_VECTOR_HPP_INCLUDED 1

#include <string>
#include <sstream>

#include <boost/numpy.hpp>

#include <cpl.h>

#include <deepskydiscovery/error.hpp>

namespace bn = boost::numpy;

namespace deepskydiscovery {

class Vector
{
  public:
    Vector()
      : owns_cpl_object_(true)
      , cpl_vector_(NULL)
    {}

    Vector(size_t n)
      : owns_cpl_object_(true)
    {
        cpl_vector_ = cpl_vector_new(n);
        if(! cpl_vector_)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not create vector of size "<<n<<": "<<msg;
            throw RuntimeError(ss.str());
        }
    }

    /** Creates a Vector which handles a cpl_vector resource.
     *  If the owns_cpl_object argument is set to ``false`` the vector will not be
     *  deallocated when this Vector object is destroyed.
     */
    Vector(
        cpl_vector * vec
      , bool owns_cpl_object=true
    )
      : owns_cpl_object_(owns_cpl_object)
      , cpl_vector_(vec)
    {}

    /** Copy constructor. It duplicates the vector data, so it will own the
     *  data.
     */
    Vector(Vector const & vec)
      : owns_cpl_object_(true)
    {
        cpl_vector_ = cpl_vector_duplicate(vec.cpl_vector_);
        if(! cpl_vector_)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not duplicate vector: "<<msg;
            throw RuntimeError(ss.str());
        }
    }

    virtual
    ~Vector()
    {
        if(cpl_vector_ && owns_cpl_object_)
        {
            cpl_vector_delete(cpl_vector_);
        }
    }

    cpl_vector *
    GetCPLVector() const
    {
        return cpl_vector_;
    }

    bool
    GetOwnsCPLObject()
    {
        return owns_cpl_object_;
    }

    size_t
    GetSize() const
    {
        return cpl_vector_get_size(cpl_vector_);
    }

    void
    Fill(double value)
    {
        cpl_error_code errcode = cpl_vector_fill(cpl_vector_, value);
        if(errcode != CPL_ERROR_NONE)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not fill vector: "<<msg;
            throw RuntimeError(ss.str());
        }
    }

    bn::ndarray
    py_get_ndarray() const
    {
        bn::dtype dt = bn::dtype::get_builtin<double>();
        std::vector<intptr_t> shape(1);
        shape[0] = GetSize();
        std::vector<intptr_t> strides(1);
        strides[0] = dt.get_itemsize();
        void * data = cpl_vector_get_data(cpl_vector_);
        return bn::from_data(data, dt, shape, strides, NULL);
    }

  protected:
    bool owns_cpl_object_;
    cpl_vector * cpl_vector_;
};

}//namespace deepskydiscovery

#endif // !DEEPSKYDISCOVERY_VECTOR_HPP_INCLUDED
