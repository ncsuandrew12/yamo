// Header
#include "Field.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

std::ostream& operator<<(std::ostream& os, const Field& e)
{
    return e.print(os);
}

Field::Field(const std::string& name, const FieldType& type, const bool override) :
    mName(name),
    mType(type)
{
    if (!override && mType.mSchemaType == FieldSchemaType::REFERENCE) {
        throw_with_trace(Exception{"Field constructed of type REFERENCE. Use RefField instead!"});
    }
}

bool Field::operator==(const Field& other) const {
    return mName == other.mName && mType == other.mType;
}

bool Field::operator!=(const Field& other) const {
    return !operator==(other);
}

std::string Field::SerializeSQLCreate() {
    std::string sql = mName + " ";
    switch (mType.mPgType) {
        case FieldPgType::SERIAL:
            sql = sql + "serial";
            break;
        case FieldPgType::VARCHAR128:
            sql = sql + "varchar(128)";
            break;
        default:
            throw_with_trace(Exception{format("Unimplemented enum value %d", mType.mPgType)});
            break;
    }
    return sql;
}

std::string Field::SerializeSQLInsert() {
    return mName;
}

std::string Field::SerializeSQLInsertData(const std::string& data) {
    std::string sql = "";
    switch (mType.mBaseType) {
        case FieldBaseType::LONG:
            sql = data;
            break;
        case FieldBaseType::STRING:
            sql = "'" + data + "'";
            break;
        default:
            throw_with_trace(Exception{format("Unimplemented enum value %d", mType.mBaseType)});
            break;
    }
    return sql;
}

std::string Field::SerializeSQLSelect() {
    return mName;
}

std::ostream& Field::print(std::ostream& os) const {
    return os << "    " << mName;
}

}