#ifndef DEEPSKYDISCOVERY_BIVECTOR_HPP_INCLUDED
#define DEEPSKYDISCOVERY_BIVECTOR_HPP_INCLUDED 1

#include <string>
#include <sstream>

#include <boost/numpy.hpp>

#include <cpl.h>

#include <deepskydiscovery/error.hpp>
#include <deepskydiscovery/vector.hpp>

namespace bn = boost::numpy;

namespace deepskydiscovery {

class BiVector
{
  public:
    /** Creates a new BiVector of size n.
     */
    BiVector(size_t n)
      : owns_cpl_object_(true)
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
        x_ = Vector(cpl_bivector_get_x(cpl_bivector_), /*owns_cpl_object=*/false);
        y_ = Vector(cpl_bivector_get_y(cpl_bivector_), /*owns_cpl_object=*/false);
    }

    /* Creates a BiVector which handles a cpl_bivector resource.
     * If the owns_cpl_object argument is set to ``false`` the bivector will not be
     * deallocated when this BiVector object is destroyed.
     */
    BiVector(
        cpl_bivector * vec
      , bool owns_cpl_object=true
    )
      : owns_cpl_object_(owns_cpl_object)
      , cpl_bivector_(vec)
      , x_(Vector(cpl_bivector_get_x(cpl_bivector_), /*owns_cpl_object=*/false))
      , y_(Vector(cpl_bivector_get_y(cpl_bivector_), /*owns_cpl_object=*/false))
    {}

    /** Copy constructor. It duplicates the bivector data, so it will own the
     *  data.
     */
    BiVector(BiVector const & vec)
      : owns_cpl_object_(true)
      , x_(vec.x_)
      , y_(vec.y_)
    {
        cpl_bivector_ = cpl_bivector_duplicate(vec.cpl_bivector_);
        if(! cpl_bivector_)
        {
            std::string msg(cpl_error_get_message());
            std::stringstream ss;
            ss << "Could not duplicate bivector: "<<msg;
            throw RuntimeError(ss.str());
        }
        x_ = Vector(cpl_bivector_get_x(cpl_bivector_), /*owns_cpl_object=*/false);
        y_ = Vector(cpl_bivector_get_y(cpl_bivector_), /*owns_cpl_object=*/false);
    }

    virtual
    ~BiVector()
    {
        if(cpl_bivector_ && owns_cpl_object_)
        {
            cpl_bivector_delete(cpl_bivector_);
        }
    }

    cpl_bivector *
    GetCPLBiVector() const
    {
        return cpl_bivector_;
    }

    size_t
    GetSize() const
    {
        return cpl_bivector_get_size(cpl_bivector_);
    }

    Vector &
    GetX()
    {
        return x_;
    }

    Vector &
    GetY()
    {
        return y_;
    }

  protected:
    bool owns_cpl_object_;
    cpl_bivector * cpl_bivector_;
    Vector x_;
    Vector y_;
};

}//namespace deepskydiscovery

#endif // !DEEPSKYDISCOVERY_BIVECTOR_HPP_INCLUDED
