#ifndef _H_EMAIL
#define _H_EMAIL

// Common
#include "common.hpp"
#include "json.hpp"

// Standard includes

// Third-party includes
#include <pqxx/pqxx>

// Yamo includes
#include "IDState.hpp"
#include "Model.hpp"

namespace Yamo {

class Email
{
public:
    EmailID mID;
    EntityID mEntityID;
    std::string mEmail;

    friend std::ostream& operator<<(std::ostream& os, const Email& e);

    Email() {
    }

    Email(const EmailID id, const EntityID entityID, const std::string& email) :
        mID(id),
        mEntityID(entityID),
        mEmail(email) {
    }

    void serializeJsonCompact(json& data);

    void serializeJsonDB(json& data);

    void deserializeDB(const pqxx::row& row);

private:
};

std::ostream& operator<<(std::ostream& os, const Email& e);

}
#endif /* _H_EMAIL */