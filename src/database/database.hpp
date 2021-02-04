#ifndef _H_DATABASE
#define _H_DATABASE

// Common
#include "../common/common.hpp"

// Standard includes
#include <iostream>

// Third-party includes
#include <pqxx/pqxx>

// Yamo includes
#include "schema.hpp"

namespace Yamo {

// Forwards
class Entity;
class DBException;
class DBQueryException;

class Database {
private:
    boost::shared_ptr<pqxx::connection> mConnection;
    const YamoSchema mSchema;

public:
    void connect(const std::string& serverType,
                 const std::string& server,
                 const std::string& user,
                 const Secret& password,
                 const std::string& schema);
    void clear();
    void setup();
    void populate(const std::map<std::string, std::vector<std::vector<std::string>>>& data);
    std::list<boost::shared_ptr<Entity>> queryEntities();

private:
    pqxx::result dbQueryEntities();
    pqxx::result dbQueryEmails();
};

}
#endif /* _H_DATABASE */