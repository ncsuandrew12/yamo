#ifndef _H_DATATYPES
#define _H_DATATYPES

// Common
#include "common.hpp"

// Standard includes

// Third-party includes
#include <pqxx/pqxx>

// Yamo includes

namespace Yamo {

typedef unsigned long DBSerial;
typedef DBSerial EntityID;
typedef std::string Email;
typedef pqxx::row EntityDBEntry;

// Forwards
class Entity;

}
#endif /* _H_DATATYPES */