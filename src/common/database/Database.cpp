// Header
#include "Database.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

Database::Config::Config(const json& config) {
    mServer = config["server"];
    mServerType = config["serverType"];
    mUser = config["user"];
    mPassword = config["password"];
    mSchemaName = config["schemaName"];
}

void Database::connect(const Config& config) {
    try {
        mConnection = boost::make_shared<pqxx::connection>(stringFormat("%s://%s:%s@%s/%s",
                                                                        config.mServerType.c_str(),
                                                                        config.mUser.c_str(),
                                                                        config.mPassword.c_str(),
                                                                        config.mServer.c_str(),
                                                                        config.mSchemaName.c_str()));
    } catch (const std::exception& e) { // Add catches for all pqxx error types
        throw_with_trace(Exception{e, "Error connecting to database: %s://%s@%s/%s",
                                       config.mServerType.c_str(),
                                       config.mUser.c_str(),
                                       config.mServer.c_str(),
                                       config.mSchemaName.c_str()});
    }
}

void Database::clear()
{
    try {
        pqxx::work txn{*mConnection};
        std::vector<boost::shared_ptr<Table>> tables = mSchema.mTablesVector;
        // Later tables may depend on earlier tables, so reverse the order when dropping.
        std::reverse(tables.begin(), tables.end());
        for (const boost::shared_ptr<Table>& table : tables) {
            try {
                txn.exec(table->SerializeSQLDrop().c_str());
            }
            catch (pqxx::sql_error const &e) {
                throw_with_trace(DBQueryException{e, "Error dropping table: %s", table->mName.c_str()});
            }
        }
        txn.commit();
    }
    catch (pqxx::sql_error const &e) {
        throw_with_trace(DBQueryException{e, "Error dropping tables from DB"});
    }
}

void Database::setup()
{
    try {
        pqxx::work txn{*mConnection};
        for (const boost::shared_ptr<Table>& table : mSchema.mTablesVector) {
            try {
                txn.exec(table->SerializeSQLCreate().c_str());
            }
            catch (pqxx::sql_error const &e) {
                throw_with_trace(DBQueryException{e, "Error creating table: %s", table->mName.c_str()});
            }
        }
        txn.commit();
    }
    catch (pqxx::sql_error const &e)
    {
        throw_with_trace(DBQueryException{e, "Error setting up DB"});
    }
}

void Database::populate(const json& data)
{
    try {
        pqxx::work txn{*mConnection};
        for (const boost::shared_ptr<Table>& table : mSchema.mTablesVector) {
            if (data.find(table->mName) == data.end()) {
                throw_with_trace(Exception{"Table %s does not exist in JSON data", table->mName.c_str()});
            }
            try {
                txn.exec(table->SerializeSQLInsert(data.at(table->mName)).c_str());
            }
            catch (pqxx::sql_error const &e) {
                throw_with_trace(DBQueryException{e, "Error populating table: %s", table->mName.c_str()});
            }
        }
        txn.commit();
    }
    catch (pqxx::sql_error const &e)
    {
        throw_with_trace(DBQueryException{e, "Error populating DB"});
    }
}

pqxx::result Database::queryTable(const std::string& tableName)
{
    return queryTable(mSchema.mTables.at(tableName));
}

pqxx::result Database::queryTable(const boost::shared_ptr<Table>& table)
{
    pqxx::result r;
    try {
        pqxx::work txn{*mConnection};
        r = pqxx::result{txn.exec(table->SerializeSQLSelect())};
        txn.commit();
        return r;
    }
    catch (pqxx::sql_error const &e)
    {
        throw_with_trace(DBQueryException{e, "Error querying DB.%s", table->mName});
    }
    return r;
}

}