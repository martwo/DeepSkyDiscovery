/**
 * $Id$
 *
 * Copyright (C)
 * 2015 - $Date$
 *     Martin Wolf <deepskydiscovery@martin-wolf.org>
 *
 */
#include <boost/preprocessor/cat.hpp>
#include <boost/python.hpp>

#include <deepskydiscovery/error.hpp>

namespace bp = boost::python;

namespace deepskydiscovery {

template <class ExcType>
static
void translate(ExcType const & e);

#define DEEPSKYDISCOVERY_ERROR_TRANSLATE(exctype)                   \
    template <>                                                     \
    void translate<exctype>(exctype const & e)                      \
    {                                                               \
        /* Use the Python 'C' API to set up an exception object. */ \
        PyErr_SetString( BOOST_PP_CAT(PyExc_, exctype), e.what() ); \
    }

DEEPSKYDISCOVERY_ERROR_TRANSLATE(AssertionError)
DEEPSKYDISCOVERY_ERROR_TRANSLATE(IndexError)
DEEPSKYDISCOVERY_ERROR_TRANSLATE(MemoryError)
DEEPSKYDISCOVERY_ERROR_TRANSLATE(RuntimeError)
DEEPSKYDISCOVERY_ERROR_TRANSLATE(TypeError)
DEEPSKYDISCOVERY_ERROR_TRANSLATE(ValueError)

#undef DEEPSKYDISCOVERY_ERROR_TRANSLATE

void register_error_types()
{
    bp::register_exception_translator<AssertionError>(&translate<AssertionError>);
    bp::register_exception_translator<IndexError>    (&translate<IndexError>);
    bp::register_exception_translator<MemoryError>   (&translate<MemoryError>);
    bp::register_exception_translator<RuntimeError>  (&translate<RuntimeError>);
    bp::register_exception_translator<TypeError>     (&translate<TypeError>);
    bp::register_exception_translator<ValueError>    (&translate<ValueError>);
}

}// namespace deepskydiscovery
