#ifndef _H_YAMOSCHEMA
#define _H_YAMOSCHEMA

// Common
#include "../common/common.hpp"
#include "../common/json.hpp"

// Standard includes

// Third-party includes

// Yamo includes
#include "schema/Schema.hpp"

namespace Yamo {

class YamoSchema : public Schema {
public:
    YamoSchema() {}
    YamoSchema(const json& schema);
};

}
#endif /* _H_YAMOSCHEMA */