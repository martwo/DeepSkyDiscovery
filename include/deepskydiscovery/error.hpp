/**
 * $Id$
 *
 * Copyright (C)
 * 2015 - $Date$
 *     Martin Wolf <deepskydiscovery@martin-wolf.org>
 *
 */
#ifndef DEEPSKYDISCOVERY_ERROR_HPP_INCLUDED
#define DEEPSKYDISCOVERY_ERROR_HPP_INCLUDED 1

#include <deepskydiscovery/detail/error.hpp>

namespace deepskydiscovery {

typedef detail::error<detail::AssertionErrorType> AssertionError;
typedef detail::error<detail::IndexErrorType>     IndexError;
typedef detail::error<detail::MemoryErrorType>    MemoryError;
typedef detail::error<detail::RuntimeErrorType>   RuntimeError;
typedef detail::error<detail::TypeErrorType>      TypeError;
typedef detail::error<detail::ValueErrorType>     ValueError;

}// namespace deepskydiscovery

#endif // ! DEEPSKYDISCOVERY_ERROR_HPP_INCLUDED
