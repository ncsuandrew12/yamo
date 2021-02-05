#ifndef _H_IDSTATE
#define _H_IDSTATE

// Common
#include "common.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

typedef unsigned long DBSerial;
typedef DBSerial EmailID;
typedef DBSerial EntityID;

struct IDState {
    EntityID mEntityID = 1;
    EmailID mEmailID = 1;
};

}
#endif /* _H_IDSTATE */