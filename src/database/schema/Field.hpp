#ifndef _H_FIELD
#define _H_FIELD

// Common
#include "../../common/common.hpp"

// Standard includes
#include <iostream>

// Third-party includes

// Yamo includes
#include "FieldType.hpp"

namespace Yamo {

class Field {
    friend std::ostream& operator<<(std::ostream& os, const Field& e);

public:
    const std::string mName;
    const FieldType mType;

    Field(const std::string& name, const FieldType& type, const bool override = false);

    bool operator==(const Field& other) const;
    bool operator!=(const Field& other) const;

    std::string SerializeSQLCreate();
    std::string SerializeSQLInsert();
    std::string SerializeSQLInsertData(const std::string& data);

    virtual std::ostream& print(std::ostream& os) const;
};

}
#endif /* _H_FIELD */