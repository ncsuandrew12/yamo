// Header
#include "Table.hpp"

// Standard includes

// Third-party includes

// Yamo includes
#include "RefField.hpp"

namespace Yamo {

std::ostream& operator<<(std::ostream& os, const Table& t)
{
    os << "  Table: " << t.mName << "(" << t.mPrimaryKey->mName << ")" << std::endl << *(t.mPrimaryKey);
    if (t.mFields.size() + t.mReferences.size() > 1) {
        os << std::endl;
    }
    for (boost::shared_ptr<RefField> field : t.mReferences) {
        os << *field;
        if (field != t.mReferences.back() || t.mFields.size() > 1) {
            os << std::endl;
        }
    }
    for (boost::shared_ptr<Field> field : t.mFields) {
        if (field == t.mPrimaryKey) {
            continue;
        }
        os << *field;
        if (field != t.mFields.back()) {
            os << std::endl;
        }
    }
    return os;
}

std::string Table::SerializeSQLCreate() const {
    std::string sql = "CREATE TABLE " + mName + "(" + mPrimaryKey->SerializeSQLCreate() + " PRIMARY KEY";
    for (boost::shared_ptr<RefField> field : mReferences) {
        sql = sql + ", " + field->SerializeSQLCreate();
    }
    for (boost::shared_ptr<Field> field : mFields) {
        if (field != mPrimaryKey) {
            sql = sql + ", " + field->SerializeSQLCreate();
        }
    }
    for (boost::shared_ptr<RefField> field : mReferences) {
        sql = sql + ", " + field->SerializeSQLRefCreate();
    }
    return sql + ")";
}

std::string Table::SerializeSQLDrop() const {
    return stringFormat("DROP TABLE IF EXISTS %s;", mName.c_str());
}

std::string Table::SerializeSQLInsert(const json& data) const {
    std::string sql = "INSERT INTO " + mName + "(" + mPrimaryKey->SerializeSQLInsert();
    for (boost::shared_ptr<RefField> field : mReferences) {
        sql = sql + ", " + field->SerializeSQLInsert();
    }
    for (boost::shared_ptr<Field> field : mFields) {
        if (field == mPrimaryKey) {
            continue;
        }
        sql = sql + ", " + field->SerializeSQLInsert();
    }
    sql = sql + ") VALUES ";
    bool first = true;
    for (const std::vector<std::string>& rowData : data) {
        if (first) {
            first = false;
        } else {
            sql = sql + ", ";
        }

        if (mReferences.size() + mFields.size() != rowData.size()) {
            throw_with_trace(Exception{"Wrong length of field entries (%d, expected %d)",
                                       rowData.size(),
                                       mReferences.size() + mFields.size()});
        }

        sql = sql + "(";
        auto datum = rowData.begin();
        sql = sql + mPrimaryKey->SerializeSQLInsertData(*datum++);
        for (boost::shared_ptr<RefField> field : mReferences) {
            sql = sql + ", " + field->SerializeSQLInsertData(*datum++);
        }
        for (boost::shared_ptr<Field> field : mFields) {
            if (field == mPrimaryKey) {
                continue;
            }
            sql = sql + ", "  + field->SerializeSQLInsertData(*datum++);
        }
        sql = sql + ")";
    }
    return sql;
}

std::string Table::SerializeSQLSelect() const {
    std::string sql = "SELECT " + mPrimaryKey->SerializeSQLSelect();
    for (boost::shared_ptr<RefField> field : mReferences) {
        sql = sql + ", " + field->SerializeSQLSelect();
    }
    for (boost::shared_ptr<Field> field : mFields) {
        if (field != mPrimaryKey) {
            sql = sql + ", " + field->SerializeSQLSelect();
        }
    }
    return sql + " FROM " + mName;
}

const std::vector<boost::shared_ptr<Field>>& Table::getFields() const {
    return mFields;
}

}