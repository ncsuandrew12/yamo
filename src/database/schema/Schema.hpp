#ifndef _H_SCHEMA
#define _H_SCHEMA

// Common
#include "../../common/common.hpp"

// Standard includes
#include <iostream>

// Third-party includes
#include <pqxx/pqxx>

// Yamo includes
#include "Field.hpp"
#include "FieldType.hpp"
#include "RefField.hpp"
#include "Table.hpp"

namespace Yamo {

class Schema {
    friend std::ostream& operator<<(std::ostream& os, const Schema& s);

public:
    static const FieldType kKey;
    static const FieldType kReference;
    static const FieldType kShortString;

    std::vector<boost::shared_ptr<Table>> mTables;
};

}
#endif /* _H_SCHEMA */