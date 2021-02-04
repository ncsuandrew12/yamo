#ifndef _H_COMMON_BASE
#define _H_COMMON_BASE

// Standard includes
// #include <cxxabi.h>

// Third-party includes
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/stacktrace.hpp>
#include <boost/make_shared.hpp>

// Yamo includes

namespace Yamo {

#define LINE_INFO __FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << ")"

typedef std::string Secret; // TODO Make non-dumpable and zeroize

// template<typename T>
// const std::string getClassName(T) {
//     int status;
//     char * demangled = abi::__cxa_demangle(typeid(T).name(),0,0,&status);
//     std::string name(demangled);
//     free(demangled);
//     return name;
// }

template<typename... Args>
std::string stringFormat(const std::string& format, const Args&... args) {
    char buffer[255]; // TODO
    snprintf(buffer, 255, format.c_str(), args...);
    return std::string(buffer);
}

}
#endif /* _H_COMMON_BASE */