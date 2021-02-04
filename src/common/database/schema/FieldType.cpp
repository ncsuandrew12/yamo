// Header
#include "FieldType.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

bool FieldType::operator==(const FieldType& other) const {
    return mSchemaType == other.mSchemaType && mPgType == other.mPgType && mBaseType == other.mBaseType;
}

}