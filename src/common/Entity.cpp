// Header
#include "Entity.hpp"

// Standard includes

// Third-party includes

// Yamo includes
#include "exceptions.hpp"

namespace Yamo {

std::ostream& operator<<(std::ostream& os, const Entity& e) {
    os << "" << e.mID << " " << e.mFirstName << " " << e.mLastName << " {";
    bool first = true;
    for (const Email& email : e.mEmails) {
        if (first) {
            first = false;
        } else {
            os << ",";
        }
        os << email;
    }
    return os << "}";
}

json Entity::serializeJsonCompact() {
    json data;
    data["firstName"] = mFirstName;
    data["lastName"] = mLastName;
    if (!mEmails.empty()) {
        data["emails"] = std::vector<std::string>();
        for (Email& email : mEmails) {
            email.serializeJsonCompact(data["emails"]);
        }
    }
    return data;
}

void Entity::deserializeJsonCompact(const json& data, IDState& idState) {
    mID = idState.mEntityID++;
    mFirstName = deserializeJsonField<std::string>(data, "firstName");
    mLastName = deserializeJsonField<std::string>(data, "lastName");
    for (const std::string& email : data["emails"]) {
        mEmails.emplace_back(idState.mEmailID++, mID, email);
    }
}

template<typename T>
T Entity::deserializeJsonField(const json& data, const std::string& fieldName) {
    if (data.find(fieldName) == data.end()) {
        throw_with_trace(Exception{"Entity JSON data missing %s field", fieldName});
    }
    return data[fieldName].get<T>();
}

void Entity::serializeJsonDBEntity(json& data) {
    data.emplace_back(std::vector<std::string>({stringFormat("%ld", mID), mFirstName, mLastName}));
}

void Entity::serializeJsonDBEmails(json& data) {
    for (Email& email : mEmails) {
        email.serializeJsonDB(data);
    }
}

void Entity::deserializeDB(const pqxx::row& entry) {
    mID = entry["entityID"].as<EntityID>();
    mFirstName = entry["firstName"].as<std::string>();
    mLastName = entry["lastName"].as<std::string>();
}

}