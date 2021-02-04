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
public:
    const std::string mMsg;
    const std::exception mEx;

    Exception(const std::exception& e) :
        Exception(e.what(), e)
    {}

    Exception(const std::string& msg, const std::exception& e) :
        std::exception(e),
        mMsg(msg),
        mEx(e)
    {}

    Exception(const std::string& msg) :
        std::exception(),
        mMsg(msg)
    {}

    friend std::ostream& operator<<(std::ostream& os, const Exception& e)
    {
        e.printMsg(os);
        e.printTrace(os);
        return os;
    }

    const char* what() const throw() override;

protected:
    virtual std::ostream& printMsg(std::ostream& os) const;
    std::ostream& printTrace(std::ostream& os) const;
};

}
#endif /* _H_EXCEPTION */