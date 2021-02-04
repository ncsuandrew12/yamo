#ifndef _H_EXCEPTION
#define _H_EXCEPTION

// Common
#include "common_base.hpp"

// Standard includes
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
#include <iostream>

// Third-party includes

// Yamo includes
#include "exceptions.hpp"

namespace Yamo {

class Exception : public std::exception
{
    friend std::ostream& operator<<(std::ostream& os, const Exception& e);

public:
    const std::string mMsg;
    const std::exception mEx;

    Exception(const std::string& msg) :
        Exception(std::exception{}, msg)
    {}

    template<typename... Args>
    Exception(const std::string& format, Args... args) :
        Exception(std::exception{}, format, args...)
    {}

    Exception(const std::exception& e) :
        Exception(e, e.what())
    {}

    Exception(const std::exception& e, const std::string& msg) :
        Exception(e, msg.c_str())
    {}

    Exception(const std::exception& e, const char* msg) :
        Exception(e, "%s", msg)
    {}

    template<typename... Args>
    Exception(const std::exception& e, const std::string& format, Args... args) :
        std::exception(e),
        mMsg(stringFormat(format, args...)),
        mEx(e)
    {}

    const char* what() const throw() override;

protected:
    virtual std::ostream& printMsg(std::ostream& os) const;
    std::ostream& printTrace(std::ostream& os) const;
};

}
#endif /* _H_EXCEPTION */