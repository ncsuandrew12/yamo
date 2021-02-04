// Header
#include "YamoModel.hpp"

// Common
#include "common/Entity.hpp"
#include "common/database/Database.hpp"
#include "common/database/DBException.hpp"
#include "common/database/DBQueryException.hpp"

// Standard includes
// #include <iostream>

// Third-party includes
// #include <pqxx/pqxx>

// Yamo includes

namespace Yamo {

const std::string YamoModel::kTableEntities = "entities";
const std::string YamoModel::kTableEmails = "emails";

void YamoModel::sync() {
    syncEntities();
}

void YamoModel::syncEntities() {
    if(mEntities.empty()) {
        pqxx::result r = mDatabase->queryTable(kTableEntities);
        for (pqxx::row row : r) {
            boost::shared_ptr<Entity> entity = boost::make_shared<Entity>(row);
            mEntities[entity->mID] = entity;
        }
        r = mDatabase->queryTable(kTableEmails);
        for (pqxx::row row : r) {
            EntityID entityID = row["entityID"].as<EntityID>();
            if (mEntities.find(entityID) != mEntities.end()) {
                mEntities[entityID]->mEmails.emplace_back(row["email"].as<Email>());
            }
            else
            {
                throw_with_trace(Exception{"Email entry lacks corresponding entity"});
            }
        }
    } else {
        mDatabase->populate(serializeJsonDB());
    }
}

json YamoModel::serializeJson() {
    json data;
    data[kTableEntities] = std::vector<json>();
    for (auto const& [key, val] : mEntities) {
        data[kTableEntities].emplace_back(val->serializeJson());
    }
    return data;
}

json YamoModel::serializeJsonDB() {
    json data;
    data[kTableEntities] = std::vector<json>();
    for (auto const& [key, val] : mEntities) {
        val->serializeJsonDBEntity(data[kTableEntities]);
    }
    data[kTableEmails] = std::vector<json>();
    for (auto const& [key, val] : mEntities) {
        val->serializeJsonDBEmails(data[kTableEmails]);
    }
    return data;
}

void YamoModel::deserializeJson(const json& data) {
    EntityID entityID = 1;
    for (const json& entityData : data[kTableEntities]) {
        boost::shared_ptr<Entity> entity = boost::make_shared<Entity>(entityData);
        entity->mID = entityID++;
        mEntities[entity->mID] = entity;
    }
}

}