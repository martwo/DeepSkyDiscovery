#include <iostream>

#include <boost/numpy.hpp>

#include <cpl.h>
#include <cpl_init.h>

#include <deepskydiscovery/core.hpp>

namespace deepskydiscovery {

void initialize()
{
    boost::numpy::initialize();
    cpl_init(CPL_INIT_DEFAULT);
}

}//namespace deepskydiscovery
