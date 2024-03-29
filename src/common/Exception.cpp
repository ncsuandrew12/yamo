// Header
#include "Exception.hpp"

// Standard includes
#include <iostream>

// Third-party includes
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

// Yamo includes

namespace Yamo {

std::ostream& operator<<(std::ostream& os, const Exception& e)
{
    e.printMsg(os);
    return e.printTrace(os);
}

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
    os << std::endl << "  Trace:" << std::endl;
    for (std::vector<boost::stacktrace::frame>::size_type sti = 1 ; sti < frames.size() ; sti++)
    {
        os << "    " << sti << "# " << frames[sti];
        if (sti < frames.size() - 1) {
            os << std::endl;
        }
    }
    return os;
}

}