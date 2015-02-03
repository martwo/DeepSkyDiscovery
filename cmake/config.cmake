#
# $Id$
#
# Copyright (C)
# 2015 - $Date$
#     Martin Wolf <deepskydiscovery@martin-wolf.org>
#
message(STATUS "Entering 'config.cmake'")

set(BUILD_SHARED_LIBS TRUE)

add_definitions(-fPIC)

link_libraries(stdc++)

set(DEEPSKYDISCOVERY_VERSION_STRING "0.1.0" CACHE STRING "The deepskydiscovery version." FORCE)

message(STATUS "+    DEEPSKYDISCOVERY_VERSION: ${DEEPSKYDISCOVERY_VERSION_STRING}")
message(STATUS "+    CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
