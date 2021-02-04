// Header
#include "YamoSchema.hpp"

// Standard includes

// Third-party includes

// Yamo includes

namespace Yamo {

YamoSchema::YamoSchema() {
    boost::shared_ptr<Field> entityID = boost::make_shared<Field>("entityID", kKey);
    boost::shared_ptr<Field> firstName = boost::make_shared<Field>("firstName", kShortString);
    boost::shared_ptr<Field> lastName = boost::make_shared<Field>("lastName", kShortString);
    boost::shared_ptr<Table> entityTable =
        boost::make_shared<Table>(
            "entities",
            std::vector<boost::shared_ptr<Field>>({entityID, firstName, lastName}),
            std::vector<boost::shared_ptr<RefField>>(),
            entityID);

    boost::shared_ptr<Field> emailID = boost::make_shared<Field>("emailID", kKey);
    boost::shared_ptr<RefField> emailRefEntityID =
        boost::make_shared<RefField>("entityID", kReference, entityTable, entityID); // TODO NOT NULL
    boost::shared_ptr<Field> email = boost::make_shared<Field>("email", kShortString);
    boost::shared_ptr<Table> emailTable =
        boost::make_shared<Table>(
            "emails",
            std::vector<boost::shared_ptr<Field>>({emailID, email}),
            std::vector<boost::shared_ptr<RefField>>({emailRefEntityID}),
            emailID);

    mTables = {entityTable, emailTable};
}

}