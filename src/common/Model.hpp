#ifndef _H_MODEL
#define _H_MODEL

// Common
#include "common.hpp"

// Standard includes

// Third-party includes
#include <pqxx/pqxx>

// Yamo includes
#include "json.hpp"

namespace Yamo {

typedef unsigned long DBSerial;
typedef DBSerial EntityID;
typedef std::string Email;
typedef pqxx::row EntityDBEntry;

// Forwards
class Database;
class Entity;

class Model {
public:
    boost::shared_ptr<Database> mDatabase;

    Model(const boost::shared_ptr<Database> database) :
        mDatabase(database)
    {}
};

}
#endif /* _H_MODEL */