#ifndef _H_DBEXCEPTION
#define _H_DBEXCEPTION

// Common
#include "../common/common.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

// Forwards
class Entity;

class DBException : public Exception {
public:
    DBException(const std::string& msg, const std::exception& e) :
        Exception(msg, e)
    {}
};

}
#endif /* _H_DBEXCEPTION */