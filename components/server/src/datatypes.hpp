#ifndef _H_DATATYPES
#define _H_DATATYPES

// Standard includes

// Third-party includes
#include <pqxx/pqxx>

// Yamo includes

namespace Yamo {

typedef unsigned long DBSerial;
typedef DBSerial EntityID;
typedef std::string Email;
typedef pqxx::row EntityDBEntry;

class Entity
{
public:
    EntityID mYamoID;
    std::string mFirstName;
    std::string mLastName;
    std::vector<Email> mEmails;

    Entity() {};
    Entity(const EntityDBEntry& entry) { Deserialize(entry); }

    friend std::ostream& operator<<(std::ostream& os, const Entity& e) {
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

    void Deserialize(const EntityDBEntry& entry);
};

}
#endif /* _H_DATATYPES */