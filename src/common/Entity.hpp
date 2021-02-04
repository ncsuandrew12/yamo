#ifndef _H_ENTITY
#define _H_ENTITY

// Common
#include "common.hpp"
#include "json.hpp"

// Standard includes

// Third-party includes

// Yamo includes
#include "Model.hpp"

namespace Yamo {

class Entity;

std::ostream& operator<<(std::ostream& os, const Entity& e);

class Entity
{
public:
    EntityID mID;
    std::string mFirstName;
    std::string mLastName;
    std::vector<Email> mEmails;

    friend std::ostream& operator<<(std::ostream& os, const Entity& e);

    Entity() {};
    Entity(const EntityDBEntry& entry) { Deserialize(entry); }
    Entity(const json& data) { deserializeJson(data); }

    json serializeJson();
    void serializeJsonDBEntity(json& data);
    void serializeJsonDBEmails(json& data);
    void Deserialize(const EntityDBEntry& entry);
    void deserializeJson(const json& data);

private:
    template<typename T>
    T deserializeField(const json& data, const std::string& fieldName);
};

}
#endif /* _H_ENTITY */