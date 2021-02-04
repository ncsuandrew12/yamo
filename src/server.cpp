// Header
#include "server.hpp"

// Common
#include "common/json.hpp"
#include "common/Entity.hpp"
#include "common/Model.hpp"
#include "common/database/Database.hpp"
#include "common/database/schema/Schema.hpp"

// Standard includes
#include <iostream>
#include <fstream>

// Third-party includes

// Yamo includes
#include "YamoModel.hpp"

using namespace Yamo;

int main(int argc, char **argv) {
    boost::shared_ptr<Database> database;
    try {
        json config;
        {
            try {
                std::ifstream file("cfg/config.json");
                file >> config;
            } catch (const json::exception& e) {
                throw_with_trace(JsonException{e, "Error parsing config."});
            }
        }

        json schemaJson;
        {
            try {
                std::ifstream file("data/schema.json");
                file >> schemaJson;
            } catch (const json::exception& e) {
                throw_with_trace(JsonException{e, "Error parsing schema."});
            }
        }

        json dataJson;
        {
            try {
                std::ifstream file("data/data.json");
                file >> dataJson;
            } catch (const json::exception& e) {
                throw_with_trace(JsonException{e, "Error parsing schema."});
            }
        }

        Schema schema{schemaJson};

        Database::Config dbConfig{config["database"]};

        database = boost::make_shared<Database>(schema);
        database->connect(dbConfig);
        database->clear();
        database->setup();


        // database->populate(dataJson);

        YamoModel yamoModel{database};

        yamoModel.deserializeJson(dataJson);

        yamoModel.sync();

        std::cout << "Schema" << std::endl
                  << schema << std::endl
                  << std::endl
                  << "Data (Compact mode):" << std::endl
                  << yamoModel.serializeJson().dump(2) << std::endl
                  << std::endl
                  << "Data (DB mode):" << std::endl
                  << yamoModel.serializeJsonDB().dump(2) << std::endl;
    }
    catch (Exception const &e)
    {
        std::cerr << "Exception: " << e << std::endl;
        return 1;
    }
    catch (std::exception const &e)
    {
        Exception ex{e};
        std::cerr << "Exception: " << e.what() << ex << std::endl;
        return 1;
    }

    try {
        if (database) {
            database->clear();
        }
    }
    catch (Exception const &e)
    {
        std::cerr << "Exception: " << e << std::endl;
        return 1;
    }
    catch (std::exception const &e)
    {
        Exception ex{e};
        std::cerr << "Exception: " << e.what() << ex << std::endl;
        return 1;
    }

    return 0;
}

namespace Yamo {

}