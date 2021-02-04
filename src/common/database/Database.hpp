#ifndef _H_DATABASE
#define _H_DATABASE

// Common
#include "../common.hpp"
#include "../json.hpp"

// Standard includes

// Third-party includes

// Yamo includes
#include "schema/Schema.hpp"
#include "DBException.hpp"
#include "DBQueryException.hpp"

namespace Yamo {

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
    void populate(const json& data);

    pqxx::result queryTable(const std::string& tableName);
    pqxx::result queryTable(const boost::shared_ptr<Table>& table);
};

}
#endif /* _H_DATABASE */