// Header
#include "server.hpp"

// Standard includes
#include <iostream>
#include <fstream>

// Third-party includes

// Yamo common
#include "common/json.hpp"
#include "common/Entity.hpp"

// Yamo database
#include "database/database.hpp"
#include "database/schema.hpp"

// Yamo includes

using namespace Yamo;

int main(int argc, char **argv) {
    try {
        std::string configData;
        {
            std::string line;
            std::ifstream file("cfg/config.json");
            if (file.is_open()) {
                while (getline(file, line)) {
                    configData = configData + line + "\n";
                }
                file.close();
            }
        }
        json config;
        try {
            config = json::parse(configData);
            // R"(
            //     {
            //         "server": "localhost",
            //         "serverType": "postgresql",
            //         "user": "afelsher",
            //         "password": "thatsnomoon",
            //         "schema": "yamo"
            //     }
            // )"_json;
        } catch (const json::exception& e) {
            throw_with_trace(JsonException{"Error parsing config.", e});
        }

        std::string server = config["server"];
        std::string serverType = config["serverType"];
        std::string user = config["user"];
        std::string password = config["password"];
        std::string schema = config["schema"];
        Database db;
        db.connect(serverType + "://" + user + ":" + password + "@" + server + "/" + schema);
        db.clear();
        db.setup();

        std::cout << YamoSchema() << std::endl;

        std::map<std::string, std::vector<std::vector<std::string>>> data = {
            {
                "entities",
                {
                    {"Andrew", "Felsher"},
                    {"Mayra", "Felsher"}
                }
            },
            {
                "emails",
                {
                    {"1", "ncsuandrew12@gmail.com"},
                    {"2", "mayracrlinares@gmail.com"}
                }
            }
        };

        db.populate(data);
        std::list<boost::shared_ptr<Entity>> entities{db.queryEntities()};

        for (boost::shared_ptr<Entity> entity : entities) {
            // std::cout << *entity;
        }
    }
    catch (Exception const &e)
    {
        std::cerr << "Exception: " << e;
        return 1;
    }
    catch (std::exception const &e)
    {
        Exception ex{e};
        std::cerr << "Exception: " << e.what() << ex;
        return 1;
    }

    return 0;
}

namespace Yamo {

}