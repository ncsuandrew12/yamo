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
    
    friend std::ostream& operator<<(std::ostream& os, const JsonException& e);

    JsonException(const nlohmann::json::exception& e, const std::string& msg) :
        Exception(e, msg),
        mJsonEx(e)
    {}

protected:
    std::ostream& printMsg(std::ostream& os) const override;
};

}
#endif /* _H_JSON_EXCEPTION */