#ifndef DEEPSKYDISCOVERY_CORE_HPP_INCLUDED
#define DEEPSKYDISCOVERY_CORE_HPP_INCLUDED 1

#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/tuple.hpp>

#include <deepskydiscovery/bivector.hpp>

namespace bp = boost::python;

namespace deepskydiscovery {

/** Initializes the library and also the CPL library.
 */
void initialize();

/** Determines the offset of each image, in order to align each image to the
 *  first (reference) image, by using a 2D cross-correlation method.
 *  It returns a tuple containing the BiVector object with the offsets for each
 *  image and a Vector containing the correlation coefficients.
 */
bp::tuple
py_geom_img_offset_fine(
    bp::list const & image_list
  , BiVector const & offset_estimates
  , BiVector const & anchors
  , size_t s_hx
  , size_t s_hy
  , size_t m_hx
  , size_t m_hy
);



}//namespace deepskydiscovery

#endif // !DEEPSKYDISCOVERY_CORE_HPP_INCLUDED
