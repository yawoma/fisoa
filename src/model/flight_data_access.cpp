#include "model/flight_data_access.h"

#include "SQLiteCpp/Database.h"
#include "utils/logger.h"
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace fisoa {
FlightDataAccess::FlightDataAccess() : m_db("") {}

FlightDataAccess::FlightDataAccess(SQLite::Database& database) : m_db(std::move(database)) {}

int FlightDataAccess::insert_flight(const Flight& flight)
{
    
    return 0;
}

int FlightDataAccess::update_flight(const Flight& flight)
{
    return 0;
}

int FlightDataAccess::delete_flight(const std::string& flight_uuid)
{
    return 0;
}

std::optional<Flight> FlightDataAccess::get_flight(const std::string& flight_uuid)
{
    return std::nullopt;
}

std::vector<Flight> FlightDataAccess::list_flights()
{
    return {};
}

void FlightDataAccess::init_database(SQLite::Database& database)
{
    m_db = std::move(database);
}

int FlightDataAccess::create_table()
{
    int result = -1;
    if (m_db.getHandle() != nullptr)
    {
        if (!m_db.tableExists("Companies"))
        {
            return result;
        }

        if (!m_db.tableExists("Flights"))
        {
            result = m_db.exec(
                "CREATE TABLE Flights ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "companyId INTEGER, "
                "uuid TEXT NOT NULL UNIQUE, "
                "codeIcao TEXT NOT NULL UNIQUE, "
                "departureTown TEXT NOT NULL, "
                "arrivalTown TEXT NOT NULL, "
                "departureDate DATE NOT NULL, "
                "arrivalDate DATE NOT NULL, "
                "economicPlaceFee FLOAT(24) NOT NULL, " /*4 bytes float*/
                "busynessPlaceFee FLOAT(24) NOT NULL, "
                "currency TEXT, "
                "CONSTRAINT fk_company FOREIGN KEY (companyId) REFERENCES Companies(id)"
                "   ON DELETE RESTRICT"
                "   ON UPDATE CASCADE"
                ")"
            );
            get_logger().info("Flights table created");
        }
        else {
            get_logger().info("Flights table found");
            result = 0;
        }
    }
    get_logger().error("Database have not been connected");
    return result;
}
} // namespace fisoa