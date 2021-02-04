// Header
#include "YamoSchema.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

YamoSchema::YamoSchema(const json& schema) {
    try {
        // Get a map of the inter-table dependencies
        std::vector<std::string> tableNames;
        for (auto tableIter = schema.begin(); tableIter != schema.end(); tableIter++) {
            std::string tableName = tableIter.key();
            json tableValue = tableIter.value();
            for (auto fieldIter = tableValue.begin() ; fieldIter != tableValue.end(); fieldIter++) {
                json fieldValue = fieldIter.value();
                if (fieldValue["type"] == "reference") {
                    std::string targetTable = fieldValue["table"];
                    bool found = false;
                    for (auto tableNameIter = tableNames.begin(); tableNameIter < tableNames.end(); tableNameIter++) {
                        if (*tableNameIter == targetTable) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        // TODO Check the target table for any references to other tables
                        tableNames.emplace_back(targetTable);
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

}