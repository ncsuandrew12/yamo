#ifndef _H_YAMOMODEL
#define _H_YAMOMODEL

// Common
#include "common/common.hpp"
#include "common/json.hpp"
#include "common/Model.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

// Forwards
class Model;

class YamoModel : public Model {
public:
    static const std::string kTableEntities;
    static const std::string kTableEmails;

    std::map<EntityID, boost::shared_ptr<Entity>> mEntities;

public:
    void sync();

    json serializeJsonCompact();
    void deserializeJsonCompact(const json& data);

    json serializeJsonDB();

private:
    void syncEntities();
};

}
#endif /* _H_YAMOMODEL */