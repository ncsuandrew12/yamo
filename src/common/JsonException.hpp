#ifndef _H_JSON_EXCEPTION
#define _H_JSON_EXCEPTION

// Common
#include "common_base.hpp"

// Standard includes

// Third-party includes
#include "nlohmann/json.hpp"

// Yamo includes
#include "exceptions.hpp"

namespace Yamo {

class JsonException : public Exception {
public:
    const nlohmann::json::exception mJsonEx;

    JsonException(const std::string& msg, const nlohmann::json::exception& e) :
        Exception(msg, e),
        mJsonEx(e)
    {}

    friend std::ostream& operator<<(std::ostream& os, const JsonException& e)
    {
        e.printMsg(os);
        e.printTrace(os);
        return os;
    }

protected:
    std::ostream& printMsg(std::ostream& os) const override;
};

}
#endif /* _H_JSON_EXCEPTION */