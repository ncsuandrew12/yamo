#ifndef _H_DATABASE
#define _H_DATABASE

// Standard includes
#include <iostream>

// Third-party includes
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/exception/all.hpp>
#include <pqxx/pqxx>

// Yamo includes
#include "exception.hpp"

namespace Yamo {

// Forwards
class Entity;

class DBException : public Exception {
public:
    DBException(const std::string& msg, const std::exception& e) :
        Exception(msg, e)
    {}
};

class DBQueryException : public DBException {
private:
    pqxx::sql_error mSqlError;

public:
    DBQueryException(const std::string& msg, const pqxx::sql_error& e) :
        DBException(msg, e),
        mSqlError(e)
    {}

private:
    virtual std::ostream& printMsg(std::ostream& os) const override;
};

class Database {
private:
    pqxx::connection mConnection;

public:
    Database(const std::string& connection) :
        mConnection(connection)
    {};

    void clear();
    void setup();
    void populate();
    std::list<boost::shared_ptr<Entity>> queryEntities();

private:
    pqxx::result dbQueryEntities();
    pqxx::result dbQueryEmails();
};

}
#endif /* _H_DATABASE */