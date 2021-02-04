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

json Entity::serializeJson() {
    json data;
    // data["entityID"] = mID;
    data["firstName"] = mFirstName;
    data["lastName"] = mLastName;
    data["emails"] = json{mEmails};
    return data;
}

void Entity::serializeJsonDBEntity(json& data) {
    data.emplace_back(std::vector<std::string>({stringFormat("%ld", mID), mFirstName, mLastName}));
}

void Entity::serializeJsonDBEmails(json& data) {
    // TODO fix JSON representiation of emails
    for (const Email& email : mEmails) {
        data.emplace_back(std::vector<std::string>({stringFormat("%ld", data.size() + 1), stringFormat("%ld", mID), email}));
    }
}

void Entity::Deserialize(const EntityDBEntry& entry) {
    mID = entry["entityID"].as<EntityID>();
    mFirstName = entry["firstName"].as<std::string>();
    mLastName = entry["lastName"].as<std::string>();
}

template<typename T>
T Entity::deserializeField(const json& data, const std::string& fieldName) {
    if (data.find(fieldName) == data.end()) {
        throw_with_trace(Exception{"Entity JSON data missing %s field", fieldName});
    }
    return data[fieldName].get<T>();
}

void Entity::deserializeJson(const json& data) {
    // mID = deserializeField<EntityID>(data, "entityID");
    mFirstName = deserializeField<std::string>(data, "firstName");
    mLastName = deserializeField<std::string>(data, "lastName");
    mEmails = deserializeField<std::vector<Email>>(data, "emails");
}

}