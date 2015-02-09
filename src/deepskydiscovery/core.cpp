#include <iostream>

#include <boost/numpy.hpp>

#include <cpl.h>
#include <cpl_init.h>

#include <deepskydiscovery/core.hpp>
#include <deepskydiscovery/bivector.hpp>
#include <deepskydiscovery/image.hpp>
#include <deepskydiscovery/vector.hpp>

namespace deepskydiscovery {

void initialize()
{
    boost::numpy::initialize();
    cpl_init(CPL_INIT_DEFAULT);
}

bp::tuple
py_geom_img_offset_fine(
    bp::list const & image_list
  , BiVector const & offset_estimates
  , BiVector const & anchors
  , size_t s_hx
  , size_t s_hy
  , size_t m_hx
  , size_t m_hy
)
{
    size_t const n = bp::len(image_list);
    cpl_imagelist * cpl_img_list = cpl_imagelist_new();
    for(size_t i=0; i<n; ++i)
    {
        Image const & img = bp::extract<Image const &>(image_list[i]);
        cpl_imagelist_set(cpl_img_list, img.GetCPLImage(), cpl_imagelist_get_size(cpl_img_list));
    }

    Vector correl(n);
    BiVector offsets(cpl_geom_img_offset_fine(
        cpl_img_list
      , offset_estimates.GetCPLBiVector()
      , anchors.GetCPLBiVector()
      , s_hx, s_hy
      , m_hx, m_hy
      , correl.GetCPLVector()
    ));

    // We need to remove each image from the image list, because otherwise the
    // CPL image list itself will deallocate the image, and the whole thing
    // would blow up, when the Image object gets destroyed.
    for(size_t i=0; i<n; ++i)
    {
        cpl_imagelist_unset(cpl_img_list, 0);
    }
    cpl_imagelist_delete(cpl_img_list);

    // Create the return tuple.
    bp::tuple ret = bp::make_tuple<BiVector, Vector>(offsets, correl);
    return ret;
}

}//namespace deepskydiscovery
