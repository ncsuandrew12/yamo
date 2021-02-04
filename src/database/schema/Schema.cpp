// Header
#include "Schema.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

const FieldType Schema::kKey = FieldType(FieldSchemaType::KEY, FieldPgType::SERIAL, FieldBaseType::LONG);
const FieldType Schema::kReference = FieldType(FieldSchemaType::REFERENCE, FieldPgType::SERIAL, FieldBaseType::LONG);
const FieldType Schema::kShortString =
    FieldType(FieldSchemaType::VARCHAR128, FieldPgType::VARCHAR128, FieldBaseType::STRING);

std::ostream& operator<<(std::ostream& os, const Schema& s)
{
    os << "Schema:" << std::endl;
    for (auto table : s.mTablesVector) {
        os << *(table);
        if (table != s.mTablesVector.back()){
            os << std::endl;
        }
    }
    return os;
}

}