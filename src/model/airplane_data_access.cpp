#include "model/airplane_data_access.h"

#include "utils/database.h"
#include "utils/logger.h"

#include "SQLiteCpp/Database.h"

#include <utility>
#include <vector>

namespace fisoa {
AirplaneDataAccess::AirplaneDataAccess() : m_db("") { init_database(); }
AirplaneDataAccess::AirplaneDataAccess(SQLite::Database& database) : m_db(std::move(database)) {}

int AirplaneDataAccess::insert_airplane(const Airplane &airplane)
{
    int result = 0;
    return result;
}

int AirplaneDataAccess::update_airplane(const Airplane &airplane)
{
    int result = 0;
    return result;
}

int AirplaneDataAccess::delete_aiplane(const std::string& airplane_uuid)
{
    int result = 0;
    return result;
}

Airplane AirplaneDataAccess::get_airplane(const std::string& airplane_uuid)
{
    Airplane airplane{};
    return airplane;
}

std::vector<Airplane> AirplaneDataAccess::list_airplanes()
{
    std::vector<Airplane> airplanes;
    return airplanes;
}

void AirplaneDataAccess::init_database()
{
    if (create_database_directory())
    {
        const std::string DB_PATH = get_database_path();
        m_db = SQLite::Database(DB_PATH, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        // Check if the airplanes table exists, if not create it
        if (!m_db.tableExists("Airplanes")) {
            m_db.exec(
                "CREATE TABLE Airplanes (id INTEGER PRIMARY KEY AUTOINCREMENT, uuid TEXT NOT NULL UNIQUE, "
                "model TEXT NOT NULL, numberOfRows INTEGER NOT NULL, numberOfColumns INTEGER NOT NULL, numberOfLanes INTEGER NOT NULL, "
                "acquisitionDate DATE)"
            );
            get_logger().info("Airplanes table created");
        }
        else {
            get_logger().info("Airplanes table found");
        }
    }
    else {
        throw std::runtime_error("Failed to create or find database directory");
    }
}
} // namespace fisoa