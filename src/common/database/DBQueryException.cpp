// Header
#include "DBQueryException.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

std::ostream& DBQueryException::printMsg(std::ostream& os) const
{
    return Exception::printMsg(os) << std::endl
        << "  " << mSqlError.what()
        << "  QUERY: " + mSqlError.query();
}

}