#ifndef _H_COMMON
#define _H_COMMON

// Standard includes
#include <ostream>

// Third-party includes

// Yamo includes
#include "common_base.hpp"
#include "exceptions.hpp"

namespace Yamo {

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& data)
{
    os << "{ ";
    bool first = true;
    for (T datum : data) {
        if (first) {
            first = false;
        } else {
            os << ", ";
        }
        os << datum;
    }
    return os << " }";
}

}
#endif /* _H_COMMON */