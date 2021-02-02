// Standard includes
#include <iostream>

// Third-party includes
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

// Yamo includes

// Header
#include "exception.hpp"

namespace Yamo {

const char* Exception::what() const throw()
{
    return mMsg.c_str();
}

std::ostream& Exception::printMsg(std::ostream& os) const
{
    return os << what();
}

std::ostream& Exception::printTrace(std::ostream& os) const
{
    const auto* detail = boost::get_error_info<traced>(*this);

    if (detail == nullptr)
    {
        return os;
    }

    std::vector<boost::stacktrace::frame> frames = detail->as_vector();
    os << "  Trace:" << std::endl;
    for (int sti = 1 ; sti < frames.size() ; sti++)
    {
        os << "    " << sti << "# " << frames[sti] << std::endl;
    }
    return os;
}

}