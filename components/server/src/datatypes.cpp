// Standard includes

// Third-party includes

// Yamo includes
#include "exception.hpp"

// Header
#include "datatypes.hpp"

namespace Yamo {

void Entity::Deserialize(const EntityDBEntry& entry) {
    mYamoID = entry["entityID"].as<EntityID>();
    mFirstName = entry["firstName"].as<std::string>();
    mLastName = entry["lastName"].as<std::string>();
}

}