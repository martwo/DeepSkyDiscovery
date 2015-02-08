#
# $Id$
#
# Copyright (C)
# 2015 - $Date$
#     Martin Wolf <deepskydiscovery@martin-wolf.org>
#
cmake_minimum_required(VERSION 2.8.3)

# Choose CMAKE_BUILD_TYPE=Release if the user does not specify it.
if(DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel.")
else()
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel.")
endif()

project(deepskydiscovery CXX)

message(STATUS "Configuring project \"${PROJECT_NAME}\"")
message(STATUS "+    project source dir: ${PROJECT_SOURCE_DIR}")

option(INSTALL_PYTHON_PACKAGE_INTO_USER_HOME
       "If set to ON, the python package \"ndhist\" will be installed into the user's home directory."
       OFF)
if(INSTALL_PYTHON_PACKAGE_INTO_USER_HOME)
    message(STATUS "+    Will install the ${PROJECT_NAME} python package into the user's home directory.")
else()
    message(STATUS "+    Will install the ${PROJECT_NAME} python package within the '${CMAKE_INSTALL_PREFIX}' directory.")
endif()

set(${PROJECT_NAME}_CONFIGURED_SUCCESSFULLY TRUE)

include(${CMAKE_SOURCE_DIR}/cmake/config.cmake)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    CACHE PATH "Output directory for shared libraries." FORCE)

include(${CMAKE_SOURCE_DIR}/cmake/add_python_module.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/find_tool.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/tools/boost.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/tools/boostnumpy.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/tools/cpl.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/tools/python.cmake)

find_boost()
find_python()
find_boostnumpy()
find_cpl()

if(NOT (BOOST_FOUND AND BOOSTNUMPY_FOUND AND PYTHON_FOUND AND CPL_FOUND))
    set(${PROJECT_NAME}_CONFIGURED_SUCCESSFULLY FALSE)
endif()

if(${PROJECT_NAME}_CONFIGURED_SUCCESSFULLY)

    list(APPEND ${PROJECT_NAME}_libdeepskydiscovery_INCLUDE_DIRS
        ${BOOST_INCLUDE_DIRS}
        ${PYTHON_INCLUDE_DIRS}
        ${BOOSTNUMPY_INCLUDE_DIRS}
        ${CPL_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
    )

    list(APPEND ${PROJECT_NAME}_libdeepskydiscovery_LIBRARIES
        ${PYTHON_LIBRARIES}
        ${BOOST_LIBRARIES}
        ${BOOSTNUMPY_LIBRARIES}
        ${CPL_LIBRARIES}
    )

    list(APPEND ${PROJECT_NAME}_libdeepskydiscovery_SOURCE_FILES
        src/deepskydiscovery/core.cpp
    )

    include_directories(
        ${${PROJECT_NAME}_libdeepskydiscovery_INCLUDE_DIRS}
    )
    add_library(deepskydiscovery
        ${${PROJECT_NAME}_libdeepskydiscovery_SOURCE_FILES}
    )
    target_link_libraries(deepskydiscovery
        ${${PROJECT_NAME}_libdeepskydiscovery_LIBRARIES}
    )

    # Install the shared library into the library directory.
    install(TARGETS deepskydiscovery
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    )

    # Install all the public header files into the include directory.
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/deepskydiscovery
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/
        FILES_MATCHING PATTERN "*.hpp"
        PATTERN ".git" EXCLUDE
    )

    add_custom_target(build_python_package
        ALL
        mkdir -p ${CMAKE_BINARY_DIR}/python
        COMMAND cp -r ${PROJECT_SOURCE_DIR}/python ${CMAKE_BINARY_DIR}/
        COMMENT "Building python package ..."
    )

    # Build the core python extension module that provides the Python
    # interface to the C++ code of ndhist.
    list(APPEND ${PROJECT_NAME}_core_LIBRARY_LIST deepskydiscovery)
    add_python_module(core ${${PROJECT_NAME}_core_LIBRARY_LIST}
        src/pybindings/image.cpp
        src/pybindings/module.cpp
    )

    message(STATUS "Project \"${PROJECT_NAME}\" configured.")
    message(STATUS "+    Now, type-in \"cd ${CMAKE_BINARY_DIR} && make && make install\"!")
    message(STATUS "+    \"${PROJECT_NAME}\" will be installed in '${CMAKE_INSTALL_PREFIX}'.")

endif()
