/**
 * $Id$
 *
 * Copyright (C)
 * 2015 - $Date$
 *     Martin Wolf <deepskydiscovery@martin-wolf.org>
 *
 */
#include <boost/python.hpp>

#include <deepskydiscovery/core.hpp>

namespace bp = boost::python;

namespace deepskydiscovery {

void register_error_types();
void register_BiVector();
void register_Image();
void register_Mask();
void register_Vector();
void register_core();

}//namespace deepskydiscovery

BOOST_PYTHON_MODULE(core)
{
    deepskydiscovery::initialize();
    deepskydiscovery::register_error_types();
    deepskydiscovery::register_BiVector();
    deepskydiscovery::register_Image();
    deepskydiscovery::register_Mask();
    deepskydiscovery::register_Vector();
    deepskydiscovery::register_core();
}
