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

Schema::Schema(const json& schema) {
    deserializeJsonCompact(schema);
}

void Schema::deserializeJsonCompact(const json& schema) {
    try {

        std::vector<std::string> tableNames;
        for (auto tableIter = schema.begin(); tableIter != schema.end(); tableIter++) {
            addTable(schema, tableNames, tableIter.key());
        }

        for (const std::string& tableName : tableNames) {
            std::vector<boost::shared_ptr<Field>> fields;
            std::vector<boost::shared_ptr<RefField>> refFields;
            boost::shared_ptr<Field> key;
            json tableValue = schema[tableName];
            for (auto fieldIter = tableValue.begin() ; fieldIter != tableValue.end(); fieldIter++) {
                std::string fieldName = fieldIter.key();

                json fieldValue = fieldIter.value();
                std::string fieldTypeStr = fieldValue["type"];

                boost::shared_ptr<FieldType> fieldType;
                if (fieldTypeStr == "key") {
                    fieldType = boost::make_shared<FieldType>(Schema::kKey);
                } else if (fieldTypeStr == "reference") {
                    fieldType = boost::make_shared<FieldType>(Schema::kReference);
                } else if (fieldTypeStr == "shortString") {
                    fieldType = boost::make_shared<FieldType>(Schema::kShortString);
                }

                if (fieldType->mSchemaType == FieldSchemaType::REFERENCE) {
                    boost::shared_ptr<Table> targetTable = mTables[fieldValue["table"]];
                    boost::shared_ptr<Field> targetField;
                    for (const boost::shared_ptr<Field>& field : targetTable->getFields()) {
                        if (field->mName == fieldName) {
                            targetField = field;
                        }
                    }
                    refFields.emplace_back(boost::make_shared<RefField>(fieldName, *fieldType, targetTable, targetField));
                } else {
                    fields.emplace_back(boost::make_shared<Field>(fieldName, *fieldType));
                    if (fieldType->mSchemaType == FieldSchemaType::KEY) {
                        key = fields.back();
                    }
                }
            }

            mTables[tableName] = boost::make_shared<Table>(tableName, fields, refFields, key);
            mTablesVector.emplace_back(mTables[tableName]);
        }
    } catch (const json::exception& e) {
        throw_with_trace(JsonException{e, "Error parsing schema."});
    }
}

void Schema::addTable(const json& schema, std::vector<std::string>& tableNames, const std::string& tableName) {
    const json& tableValue = schema.at(tableName);
    for (auto fieldIter = tableValue.begin() ; fieldIter != tableValue.end(); fieldIter++) {
        json fieldValue = fieldIter.value();
        if (fieldValue["type"] == "reference") {
            std::string targetTable = fieldValue["table"];
            std::string targetField = fieldIter.key();
            bool found = false;
            for (const std::string& targetTableName : tableNames) {
                if (targetTableName == targetTable) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (schema.find(targetTable) == schema.end()) {
                    throw_with_trace(Exception{"Referenced table '%s' does not exist in schema", targetTable.c_str()});
                }
                if (schema[targetTable].find(targetField) == schema[targetTable].end()) {
                    throw_with_trace(Exception{"Referenced field '%s.%s' does not exist in schema",
                                               targetTable.c_str(),
                                               targetField.c_str()});
                }
                addTable(schema, tableNames, targetTable);
            }
        }
    }
    bool found = false;
    for (auto tableNameIter = tableNames.begin(); tableNameIter < tableNames.end(); tableNameIter++) {
        if (*tableNameIter == tableName) {
            found = true;
            break;
        }
    }
    if (!found) {
        tableNames.emplace_back(tableName);
    }
}

}