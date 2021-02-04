// Header
#include "FieldType.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

bool FieldType::operator==(const FieldType& other) const {
    return mYamoType == other.mYamoType && mPgType == other.mPgType && mBaseType == other.mBaseType;
}

}