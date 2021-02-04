#ifndef _H_DBEXCEPTION
#define _H_DBEXCEPTION

// Common
#include "../common.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

// Forwards
class Entity;

class DBException : public Exception {
public:
    template<typename... Args>
    DBException(const std::exception& e, const std::string& format, Args... args) :
        Exception(e, format, args...)
    {}
};

}
#endif /* _H_DBEXCEPTION */