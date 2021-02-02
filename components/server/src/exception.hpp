#ifndef _H_EXCEPTION
#define _H_EXCEPTION

// Standard includes
#include <iostream>
// #include <cxxabi.h>

// Third-party includes
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
#include <pqxx/pqxx>

// Yamo includes

namespace Yamo {

typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;

template <class E>
void throw_with_trace(const E& e) {
    throw boost::enable_error_info(e) << traced(boost::stacktrace::stacktrace());
}

// template<typename T>
// const std::string getClassName(T) {
//     int status;
//     char * demangled = abi::__cxa_demangle(typeid(T).name(),0,0,&status);
//     std::string name(demangled);
//     free(demangled);
//     return name;
// }

class Exception : public std::exception
{
public:
    const std::string mMsg;

    Exception(const std::exception& e) :
        Exception(e.what(), e)
    {}

    Exception(const std::string& msg, const std::exception& e) :
        std::exception(e),
        mMsg(msg)
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