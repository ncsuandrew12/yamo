// Standard includes
#include <iostream>

// Third-party includes
#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
#include <pqxx/pqxx>

// Yamo includes
#include "database.hpp"
#include "datatypes.hpp"
#include "exception.hpp"

// Header
#include "server.hpp"

using namespace Yamo;

int main(int argc, char **argv) {
    try
    {
        Database db("postgresql://afelsher:password@localhost/yamo");
        db.clear();
        db.setup();
        db.populate();
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