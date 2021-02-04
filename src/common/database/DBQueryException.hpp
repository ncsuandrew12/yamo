#ifndef _H_DBQUERYEXCEPTION
#define _H_DBQUERYEXCEPTION

// Common
#include "../common.hpp"

// Standard includes
#include <iostream>

// Third-party includes
#include <pqxx/pqxx>

// Yamo includes
#include "DBException.hpp"

namespace Yamo {

// Forwards
class Entity;

class DBQueryException : public DBException {
private:
    pqxx::sql_error mSqlError;

public:

    template<typename... Args>
    DBQueryException(const pqxx::sql_error& e, const std::string& format, Args... args) :
        DBException(e, format, args...),
        mSqlError(e)
    {}

private:
    virtual std::ostream& printMsg(std::ostream& os) const override;
};

}
#endif /* _H_DBQUERYEXCEPTION */