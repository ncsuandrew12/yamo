// Header
#include "RefField.hpp"

// Standard includes

// Third-party includes

// Yamo includes
#include "Table.hpp"

namespace Yamo {

std::ostream& operator<<(std::ostream& os, const RefField& e) {
    return e.print(os);
}

RefField::RefField(const std::string& name,
                   const FieldType& type,
                   const boost::shared_ptr<Table> table,
                   const boost::shared_ptr<Field> field) :
    Field(name, type, true),
    mRefTable(table),
    mRefField(field)
{
    if (mType.mYamoType != FieldSchemaType::REFERENCE) {
        throw_with_trace(Exception{"RefField constructed with type other than REFERENCE. Use Field instead!"});
    }
}

std::string RefField::SerializeSQLRefCreate() const {
    return "FOREIGN KEY (" + mName + ") REFERENCES " + mRefTable->mName + " (" + mRefField->mName + ")";
}

std::ostream& RefField::print(std::ostream& os) const {
    return Field::print(os) << " -> " << mRefTable->mName << "." << mRefField->mName;
}

}