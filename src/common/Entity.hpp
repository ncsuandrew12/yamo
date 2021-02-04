#ifndef _H_ENTITY
#define _H_ENTITY

// Common
#include "common.hpp"

// Standard includes
#include <ostream>

// Third-party includes

// Yamo includes
#include "datatypes.hpp"

namespace Yamo {

class Entity;

std::ostream& operator<<(std::ostream& os, const Entity& e);

class Entity
{
public:
    EntityID mYamoID;
    std::string mFirstName;
    std::string mLastName;
    std::vector<Email> mEmails;

    friend std::ostream& operator<<(std::ostream& os, const Entity& e);

    Entity() {};
    Entity(const EntityDBEntry& entry) { Deserialize(entry); }

    void Deserialize(const EntityDBEntry& entry);
};

}
#endif /* _H_ENTITY */