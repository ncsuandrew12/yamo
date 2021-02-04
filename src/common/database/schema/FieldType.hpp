#ifndef _H_FIELD_TYPE
#define _H_FIELD_TYPE

// Common
#include "../../common.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

enum class FieldBaseType {
    LONG,
    STRING
};

enum class FieldPgType {
    SERIAL,
    VARCHAR128
};

enum class FieldSchemaType { // TODO come up with a better name for this enum
    KEY,
    REFERENCE,
    VARCHAR128
};

class FieldType {
public:
    const FieldSchemaType mSchemaType;
    const FieldPgType mPgType;
    const FieldBaseType mBaseType;

    FieldType(const FieldSchemaType schemaType, const FieldPgType pgType, const FieldBaseType baseType) :
        mSchemaType(schemaType),
        mPgType(pgType),
        mBaseType(baseType)
    {}

    bool operator==(const FieldType& other) const;
};

}
#endif /* _H_FIELD_TYPE */