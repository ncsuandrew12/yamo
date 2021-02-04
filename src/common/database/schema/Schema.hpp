#ifndef _H_SCHEMA
#define _H_SCHEMA

// Common
#include "../../common.hpp"
#include "../../json.hpp"

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

    std::map<std::string, boost::shared_ptr<Table>> mTables;
    std::vector<boost::shared_ptr<Table>> mTablesVector;

    Schema() {}
    Schema(const json& schema);

    void deserializeJson(const json& schema);

private:
    static void addTable(const json& schema, std::vector<std::string>& tableNames, const std::string& tableName);
};

}
#endif /* _H_SCHEMA */