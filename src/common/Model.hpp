#ifndef _H_MODEL
#define _H_MODEL

// Common
#include "common.hpp"

// Standard includes

// Third-party includes

// Yamo includes
#include "json.hpp"
#include "IDState.hpp"

namespace Yamo {

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