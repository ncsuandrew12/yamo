// Standard includes
#include <iostream>

// Third-party includes
#include <boost/exception/all.hpp>
#include <boost/make_shared.hpp>
#include <pqxx/pqxx>

// Yamo includes
#include "exception.hpp"
#include "datatypes.hpp"

// Header
#include "database.hpp"

namespace Yamo {

std::ostream& DBQueryException::printMsg(std::ostream& os) const
{
    return Exception::printMsg(os) << std::endl
        << "  " << mSqlError.what()
        << "  QUERY: " + mSqlError.query() << std::endl;
}

void Database::clear()
{
    try {
        pqxx::work txn{mConnection};
        txn.exec("DROP TABLE IF EXISTS emails");
        txn.exec("DROP TABLE IF EXISTS entities");
        // txn.exec("DROP TABLE bobbity");
        txn.commit();
    }
    catch (pqxx::sql_error const &e) {
        throw_with_trace(DBQueryException{"Error clearing tables from DB", e});
    }
}

void Database::setup()
{
    try {
        pqxx::work txn{mConnection};
        txn.exec("CREATE TABLE entities(entityID serial PRIMARY KEY, firstName VARCHAR(128), lastName VARCHAR(128))");
        txn.exec("CREATE TABLE emails(emailID serial PRIMARY KEY, entityID serial NOT NULL, email VARCHAR(128), FOREIGN KEY (entityID) REFERENCES entities (entityID))");
        txn.commit();
    }
    catch (pqxx::sql_error const &e)
    {
        throw_with_trace(DBQueryException{"Error setting up DB", e});
    }
}

void Database::populate()
{
    try {
        pqxx::work txn{mConnection};
        txn.exec("INSERT INTO entities(firstName, lastName) VALUES ('Andrew', 'Felsher'), ('Mayra', 'Felsher')");
        txn.exec("INSERT INTO emails(entityID, email) VALUES (1, 'ncsuandrew12@gmail.com'), (2, 'mayracrlinares@gmail.com')");
        txn.commit();
    }
    catch (pqxx::sql_error const &e)
    {
        throw_with_trace(DBQueryException{"Error populating DB", e});
    }
}

std::list<boost::shared_ptr<Entity>> Database::queryEntities()
{
    std::map<EntityID, boost::shared_ptr<Entity>> entities;
    pqxx::result r = dbQueryEntities();
    for (pqxx::row row : r) {
        boost::shared_ptr<Entity> entity = boost::make_shared<Entity>(row);
        entities[entity->mYamoID] = entity;
    }
    r = dbQueryEmails();
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

pqxx::result Database::dbQueryEntities()
{
    pqxx::result r;
    try {
        pqxx::work txn{mConnection};
        r = pqxx::result{txn.exec("SELECT entityID, firstName, lastName FROM entities")};
        txn.commit();
        return r;
    }
    catch (pqxx::sql_error const &e)
    {
        throw_with_trace(DBQueryException{"Error querying DB.entites", e});
    }
    return r;
}

pqxx::result Database::dbQueryEmails()
{
    pqxx::result r;
    try {
        pqxx::work txn{mConnection};
        r = pqxx::result{txn.exec("SELECT entityID, email FROM emails")};
        txn.commit();
        return r;
    }
    catch (pqxx::sql_error const &e)
    {
        throw_with_trace(DBQueryException{"Error querying DB.emails", e});
    }
    return r;
}

}