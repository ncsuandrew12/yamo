#ifndef _H_REFFIELD
#define _H_REFFIELD

// Common
#include "../../common/common.hpp"

// Standard includes
#include <iostream>

// Third-party includes
#include <pqxx/pqxx>

// Yamo includes
#include "Field.hpp"
#include "FieldType.hpp"

namespace Yamo {

// Forwards
class Table;

class RefField : public Field {
    friend std::ostream& operator<<(std::ostream& os, const RefField& e);

public:
    boost::shared_ptr<Table> mRefTable;
    boost::shared_ptr<Field> mRefField;

    RefField(const std::string& name,
             const FieldType& type,
             const boost::shared_ptr<Table> table,
             const boost::shared_ptr<Field> field);

    std::string SerializeSQLRefCreate() const;

    std::ostream& print(std::ostream& os) const override;
};

}
#endif /* _H_REFFIELD */