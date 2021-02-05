#ifndef _H_ENTITY
#define _H_ENTITY

// Common
#include "common.hpp"
#include "json.hpp"

// Standard includes

// Third-party includes
#include <pqxx/pqxx>

// Yamo includes
#include "Email.hpp"
#include "IDState.hpp"
#include "Model.hpp"

namespace Yamo {

std::ostream& operator<<(std::ostream& os, const Entity& e);

class Entity
{
public:
    EntityID mID;
    std::string mFirstName;
    std::string mLastName;
    std::vector<Email> mEmails;

    friend std::ostream& operator<<(std::ostream& os, const Entity& e);

    Entity() {
    }

    json serializeJsonCompact();
    void deserializeJsonCompact(const json& data, IDState& idState);

    void serializeJsonDBEntity(json& data);
    void serializeJsonDBEmails(json& data);

    void deserializeDB(const pqxx::row& entry);

private:
    template<typename T>
    T deserializeJsonField(const json& data, const std::string& fieldName);
};

}
#endif /* _H_ENTITY */