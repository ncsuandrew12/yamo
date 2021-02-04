#ifndef _H_DATABASE
#define _H_DATABASE

// Common
#include "../common/common.hpp"
#include "../common/json.hpp"

// Standard includes
#include <iostream>

// Third-party includes
#include <pqxx/pqxx>

// Yamo includes
#include "schema/Schema.hpp"

namespace Yamo {

// Forwards
class Entity;
class DBException;
class DBQueryException;

class Database {
public:
    struct Config {
        std::string mServer;
        std::string mServerType;
        std::string mUser;
        Secret mPassword;
        std::string mSchemaName;

        Config(const json& config);
    };

public:
    static const std::string kTableEntities;
    static const std::string kTableEmails;

private:
    boost::shared_ptr<pqxx::connection> mConnection;
    const Schema mSchema;

public:
    Database(const Schema& schema) :
        mSchema(schema)
    {}

    void connect(const Config& config);
    void clear();
    void setup();
    void populate(const std::map<std::string, std::vector<std::vector<std::string>>>& data);
    std::list<boost::shared_ptr<Entity>> queryEntities();

private:
    pqxx::result queryTable(const std::string& tableName);
    pqxx::result queryTable(const boost::shared_ptr<Table>& table);
};

}
#endif /* _H_DATABASE */