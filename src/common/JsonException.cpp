// Header
#include "JsonException.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

std::ostream& JsonException::printMsg(std::ostream& os) const
{
    return os << what() << std::endl << "  Ex: " << mJsonEx.what();
}

}