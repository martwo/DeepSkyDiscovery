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

void register_Image();

}//namespace deepskydiscovery

BOOST_PYTHON_MODULE(core)
{
    deepskydiscovery::initialize();
    deepskydiscovery::register_Image();
}
