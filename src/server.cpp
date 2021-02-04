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
#include "database/Database.hpp"
#include "database/YamoSchema.hpp"

// Yamo includes

using namespace Yamo;

int main(int argc, char **argv) {
    try {
        json config;
        {
            try {
                std::ifstream file("cfg/config.json");
                file >> config;
            } catch (const json::exception& e) {
                throw_with_trace(JsonException{"Error parsing config.", e});
            }
        }

        json schemaJson;
        {
            try {
                std::ifstream file("data/schema.json");
                file >> schemaJson;
            } catch (const json::exception& e) {
                throw_with_trace(JsonException{"Error parsing schema.", e});
            }
        }

        std::map<std::string, std::vector<std::vector<std::string>>> data;
        {
            std::string table;
            std::string line;
            std::ifstream file("data/data.csv");
            if (file.is_open()) {
                while (getline(file, line)) {
                    if (line.size() == 0) {
                        continue;
                    }
                    if (line[0] == '#') {
                        table = line.substr(1);
                        data[table] = std::vector<std::vector<std::string>>();
                        continue;
                    }
                    std::string delim = ",";
                    auto fieldStart = 0;
                    auto fieldEnd = line.find(delim);
                    std::vector<std::string> rowData;
                    while (fieldEnd != std::string::npos) {
                        rowData.emplace_back(line.substr(fieldStart, fieldEnd - fieldStart));
                        fieldStart = fieldEnd + delim.length();
                        fieldEnd = line.find(delim, fieldStart);
                        if (fieldEnd == std::string::npos) {
                            rowData.emplace_back(line.substr(fieldStart, rowData.size() - fieldStart));
                        }
                    }
                    data[table].emplace_back(rowData);
                }
                file.close();
            }
        }

        YamoSchema schema(schemaJson);

        Database::Config dbConfig(config["database"]);

        Database db(schema);
        db.connect(dbConfig);
        db.clear();
        db.setup();

        std::cout << schema << std::endl;

        db.populate(data);
        std::list<boost::shared_ptr<Entity>> entities{db.queryEntities()};

        for (boost::shared_ptr<Entity> entity : entities) {
            std::cout << *entity;
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