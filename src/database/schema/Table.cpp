// Header
#include "Table.hpp"

// Standard includes

// Third-party includes

// Yamo includes
#include "RefField.hpp"

namespace Yamo {

std::ostream& operator<<(std::ostream& os, const Table& t)
{
    os << "  Table: " << t.mName << "(" << t.mPrimaryKey->mName << ")" << std::endl << *(t.mPrimaryKey) << std::endl;
    for (boost::shared_ptr<RefField> field : t.mReferences) {
        os << *field;
        if (field != t.mReferences.back() || !t.mFields.empty()) {
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

std::string Table::SerializeSQLInsert(const std::vector<std::vector<std::string>>& data) const {
    std::string sql = "INSERT INTO " + mName + "(";
    bool first = true;
    for (boost::shared_ptr<RefField> field : mReferences) {
        if (first) {
            first = false;
        } else {
            sql = sql + ", ";
        }
        sql = sql + field->SerializeSQLInsert();
    }
    for (boost::shared_ptr<Field> field : mFields) {
        if (field == mPrimaryKey) {
            continue;
        }
        if (first) {
            first = false;
        } else {
            sql = sql + ", ";
        }
        sql = sql + field->SerializeSQLInsert();
    }
    sql = sql + ") VALUES ";
    first = true;
    for (const std::vector<std::string>& rowData : data) {
        if (first) {
            first = false;
        } else {
            sql = sql + ", ";
        }

        if (mReferences.size() + mFields.size() - 1 != rowData.size()) {
            throw_with_trace(Exception{format("Wrong length of field entries (%d, expected %d)",
                                              rowData.size(),
                                              mReferences.size() + mFields.size() - 1)});
        }

        sql = sql + "(";
        auto datum = rowData.begin();
        bool firstI = true;
        for (boost::shared_ptr<RefField> field : mReferences) {
            if (firstI) {
                firstI = false;
            } else {
                sql = sql + ", ";
            }
            sql = sql + field->SerializeSQLInsertData(*datum++);
        }
        for (boost::shared_ptr<Field> field : mFields) {
            if (field == mPrimaryKey) {
                continue;
            }
            if (firstI) {
                firstI = false;
            } else {
                sql = sql + ", ";
            }
            sql = sql + field->SerializeSQLInsertData(*datum++);
        }
        sql = sql + ")";
    }
    return sql;
}

}