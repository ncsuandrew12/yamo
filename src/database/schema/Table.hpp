#ifndef _H_TABLE
#define _H_TABLE

// Common
#include "../../common/common.hpp"

// Standard includes
#include <iostream>

// Third-party includes
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/exception/all.hpp>
#include <pqxx/pqxx>

// Yamo includes
#include "Field.hpp"
#include "FieldType.hpp"

namespace Yamo {

// Forwards
class RefField;

class Table {
    friend std::ostream& operator<<(std::ostream& os, const Table& t);

public:
    const std::string mName;

private:
    // TODO combine these two vectors using polymorphism.
    std::vector<boost::shared_ptr<Field>> mFields;
    std::vector<boost::shared_ptr<RefField>> mReferences;
    boost::shared_ptr<Field> mPrimaryKey;

public:
    Table(const std::string& name,
          const std::vector<boost::shared_ptr<Field>>& fields,
          const std::vector<boost::shared_ptr<RefField>>& refs,
          const boost::shared_ptr<Field>& key) :
        mName(name),
        mFields(fields),
        mReferences(refs),
        mPrimaryKey(key)
    {}

    std::string SerializeSQLCreate() const;
    std::string SerializeSQLInsert(const std::vector<std::vector<std::string>>& data) const;

};

}
#endif /* _H_TABLE */