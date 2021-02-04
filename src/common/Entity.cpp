// Header
#include "Entity.hpp"

// Standard includes

// Third-party includes

// Yamo includes
#include "exceptions.hpp"

namespace Yamo {

std::ostream& operator<<(std::ostream& os, const Entity& e) {
    os << "" << e.mYamoID << " " << e.mFirstName << " " << e.mLastName << " {";
    bool first = true;
    for (const Email& email : e.mEmails) {
        if (first) {
            first = false;
        } else {
            os << ",";
        }
        os << email;
    }
    return os << "}" << std::endl;
}

void Entity::Deserialize(const EntityDBEntry& entry) {
    mYamoID = entry["entityID"].as<EntityID>();
    mFirstName = entry["firstName"].as<std::string>();
    mLastName = entry["lastName"].as<std::string>();
}

}