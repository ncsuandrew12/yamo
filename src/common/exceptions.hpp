#ifndef _H_EXCEPTIONS
#define _H_EXCEPTIONS

// Common
#include "common_base.hpp"

// Standard includes
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

// Third-party includes

// Yamo includes
#include "Exception.hpp"

namespace Yamo {

// Forwards
class JsonException;

typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;

template <class E>
void throw_with_trace(const E& e) {
    throw boost::enable_error_info(e) << traced(boost::stacktrace::stacktrace());
}

}
#endif /* _H_EXCEPTIONS */