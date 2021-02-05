// Header
#include "Email.hpp"

// Standard includes

// Third-party includes

// Yamo includes
#include "exceptions.hpp"

namespace Yamo {

void Email::serializeJsonCompact(json& data) {
    data.emplace_back(mEmail);
}

void Email::serializeJsonDB(json& data) {
    data.emplace_back(std::vector<std::string>({stringFormat("%ld", mID), stringFormat("%ld", mEntityID), mEmail}));
}

void Email::deserializeDB(const pqxx::row& entry) {
    mID = entry["emailID"].as<EntityID>();
    mEntityID = entry["entityID"].as<EmailID>();
    mEmail = entry["email"].as<std::string>();
}

std::ostream& operator<<(std::ostream& os, const Email& e) {
    return os << "" << e.mID << " " << e.mEntityID << " " << e.mEmail;
}

}