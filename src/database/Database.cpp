// Header
#include "Database.hpp"

// Standard includes
#include <iostream>

// Third-party includes
#include <pqxx/pqxx>

// Yamo includes
#include "../common/Entity.hpp"
#include "DBException.hpp"
#include "DBQueryException.hpp"

namespace Yamo {

const std::string Database::kTableEntities = "entities";
const std::string Database::kTableEmails = "emails";

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

void Database::populate(const std::map<std::string, std::vector<std::vector<std::string>>>& data)
{
    try {
        pqxx::work txn{*mConnection};
        for (const boost::shared_ptr<Table>& table : mSchema.mTablesVector) {
            try {
                txn.exec(
                    table->SerializeSQLInsert(std::vector<std::vector<std::string>>({data.at(table->mName)})).c_str());
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

std::list<boost::shared_ptr<Entity>> Database::queryEntities()
{
    std::map<EntityID, boost::shared_ptr<Entity>> entities;
    pqxx::result r = queryTable(kTableEntities);
    for (pqxx::row row : r) {
        boost::shared_ptr<Entity> entity = boost::make_shared<Entity>(row);
        entities[entity->mYamoID] = entity;
    }
    r = queryTable(kTableEmails);
    for (pqxx::row row : r) {
        EntityID entityID = row["entityID"].as<EntityID>();
        if (entities.find(entityID) != entities.end()) {
            entities[entityID]->mEmails.emplace_back(row["email"].as<Email>());
        }
        else
        {
            throw_with_trace(Exception{"Email entry lacks corresponding entity"});
        }
    }
    std::list<boost::shared_ptr<Entity>> list;
    for (auto entry : entities)
    {
        list.emplace_back(entry.second);
    }
    return list;
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