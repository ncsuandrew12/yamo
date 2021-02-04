#ifndef _H_SCHEMA
#define _H_SCHEMA

// Common
#include "../common/common.hpp"

// Standard includes
#include <iostream>

// Third-party includes
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/exception/all.hpp>
#include <pqxx/pqxx>

// Yamo includes

namespace Yamo {

enum class FieldBaseType {
    LONG,
    STRING
};

enum class FieldPgType {
    SERIAL,
    VARCHAR128
};

enum class FieldYamoType {
    KEY,
    REFERENCE,
    VARCHAR128
};

class FieldType {
public:
    const FieldYamoType mYamoType;
    const FieldPgType mPgType;
    const FieldBaseType mBaseType;

    FieldType(const FieldYamoType yamoType, const FieldPgType pgType, const FieldBaseType baseType) :
        mYamoType(yamoType),
        mPgType(pgType),
        mBaseType(baseType)
    {}

    bool operator==(const FieldType& other) const {
        return mYamoType == other.mYamoType && mPgType == other.mPgType && mBaseType == other.mBaseType;
    }
};

class Field {
public:
    const std::string mName;
    const FieldType mType;

    Field(const std::string& name, const FieldType& type, const bool override = false) :
        mName(name),
        mType(type)
    {
        if (!override && mType.mYamoType == FieldYamoType::REFERENCE) {
            throw_with_trace(Exception{"Field constructed of type REFERENCE. Use RefField instead!"});
        }
    }

    bool operator==(const Field& other) const {
        return mName == other.mName && mType == other.mType;
    }

    bool operator!=(const Field& other) const {
        return !operator==(other);
    }

    std::string SerializeSQLCreate() {
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

    std::string SerializeSQLInsert() {
        return mName;
    }

    std::string SerializeSQLInsertData(const std::string& data) {
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

    virtual std::ostream& print(std::ostream& os) const {
        return os << "    " << mName;
    }

    friend std::ostream& operator<<(std::ostream& os, const Field& e)
    {
        return e.print(os);
    }
};

// Forwards
class RefField;

class Table {
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

    friend std::ostream& operator<<(std::ostream& os, const Table& t);
};

class RefField : public Field {
public:
    boost::shared_ptr<Table> mRefTable;
    boost::shared_ptr<Field> mRefField;

    RefField(const std::string& name,
             const FieldType& type,
             const boost::shared_ptr<Table> table,
             const boost::shared_ptr<Field> field) :
        Field(name, type, true),
        mRefTable(table),
        mRefField(field)
    {
        if (mType.mYamoType != FieldYamoType::REFERENCE) {
            throw_with_trace(Exception{"RefField constructed with type other than REFERENCE. Use Field instead!"});
        }
    }

    std::string SerializeSQLRefCreate() const {
        return "FOREIGN KEY (" + mName + ") REFERENCES " + mRefTable->mName + " (" + mRefField->mName + ")";
    }

    std::ostream& print(std::ostream& os) const override {
        return Field::print(os) << " -> " << mRefTable->mName << "." << mRefField->mName;
    }

    friend std::ostream& operator<<(std::ostream& os, const RefField& e)
    {
        return e.print(os);
    }
};

class YamoSchema {
public:
    static const FieldType kKey;
    static const FieldType kReference;
    static const FieldType kShortString;

    std::vector<boost::shared_ptr<Table>> mTables;

    YamoSchema() {
        boost::shared_ptr<Field> entityID = boost::make_shared<Field>("entityID", kKey);
        boost::shared_ptr<Field> firstName = boost::make_shared<Field>("firstName", kShortString);
        boost::shared_ptr<Field> lastName = boost::make_shared<Field>("lastName", kShortString);
        boost::shared_ptr<Table> entityTable =
            boost::make_shared<Table>(
                "entities",
                std::vector<boost::shared_ptr<Field>>({entityID, firstName, lastName}),
                std::vector<boost::shared_ptr<RefField>>(),
                entityID);

        boost::shared_ptr<Field> emailID = boost::make_shared<Field>("emailID", kKey);
        boost::shared_ptr<RefField> emailRefEntityID =
            boost::make_shared<RefField>("entityID", kReference, entityTable, entityID); // TODO NOT NULL
        boost::shared_ptr<Field> email = boost::make_shared<Field>("email", kShortString);
        boost::shared_ptr<Table> emailTable =
            boost::make_shared<Table>(
                "emails",
                std::vector<boost::shared_ptr<Field>>({emailID, email}),
                std::vector<boost::shared_ptr<RefField>>({emailRefEntityID}),
                emailID);

        mTables = {entityTable, emailTable};
    }

    friend std::ostream& operator<<(std::ostream& os, const YamoSchema& s)
    {
        os << "Yamo:" << std::endl;
        for (boost::shared_ptr<Table> table : s.mTables) {
            os << *table << std::endl;
        }
        return os;
    }
};

}
#endif /* _H_SCHEMA */