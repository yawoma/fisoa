#include "model/flight_data_access.h"

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Exception.h"
#include "SQLiteCpp/Statement.h"
#include "utils/logger.h"
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace fisoa {
FlightDataAccess::FlightDataAccess() : m_db("") {}

FlightDataAccess::FlightDataAccess(SQLite::Database& database) : m_db(std::move(database)) {}

int FlightDataAccess::insert_flight(const Flight& flight, const std::string& company_name)
{
    SQLite::Statement query(m_db, 
        "INSERT INTO Flights "
        "(companyId, uuid, codeIcao, flightNumber, departureTown, arrivalTown, "
        "departureDate, arrivalDate, economicPlaceFee, busynessPlaceFee, currrency)"
        "VALUES ((SELECT company.id FROM Companies company WHERE company.name = ?), ?, "
        "(SELECT company.codeIcao FROM Companies company WHERE company.name = ?), ?, ?, ?, ?, ?, ?, ?, ?)"
        );
    const int COMPANY_ID_VAL = 1;
    query.bind(COMPANY_ID_VAL, company_name);
    const int UUID_VAL = 2;
    query.bind(UUID_VAL, flight.uuid);
    const int CODEICAO_VAL = 3;
    query.bind(CODEICAO_VAL, company_name);
    const int NUMBER_VAL = 4;
    query.bind(NUMBER_VAL, flight.number);
    const int DEPARTURE_TOWN_VAL = 5;
    query.bind(DEPARTURE_TOWN_VAL, flight.departureTown);
    const int ARRIVAL_TOWN_VAL = 6;
    query.bind(ARRIVAL_TOWN_VAL, flight.arrivalTown);
    const int DEPARTURE_DATE_VAL = 7;
    query.bind(DEPARTURE_DATE_VAL, flight.departureDate);
    const int ARRIVAL_DATE_VAL = 8;
    query.bind(ARRIVAL_DATE_VAL, flight.arrivalDate);
    const int ECO_VAL = 9;
    query.bind(ECO_VAL, flight.economicFee);
    const int BUSY_VAL = 10;
    query.bind(BUSY_VAL, flight.busynessFee);
    const int CURRENCY_VAL = 11;
    query.bind(CURRENCY_VAL, flight.currency);
    int result = 0;
    try {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info("Flight " + flight.number + " has been inserted successfully");
        }
    } catch (const SQLite::Exception& e) {
        get_logger().error("Failed to insert flight number " + flight.number + " : " + e.what());
    }
    return result;
}

int FlightDataAccess::update_flight(const Flight& flight)
{
    SQLite::Statement query(m_db, 
        "UPDATE Flights SET flightNumber = ?, "
        "departureTown = ?, arrivalTown = ?, "
        "departureDate = ?, arrivalDate = ?, "
        "economicPlaceFee = ?, busynessPlaceFee = ?, "
        "currency = ? WHERE uuid = ?");
    const int NUMBER_VAL = 1;
    query.bind(NUMBER_VAL, flight.number);
    const int DEPARTURE_TOWN_VAL = 2;
    query.bind(DEPARTURE_TOWN_VAL, flight.departureTown);
    const int ARRIVAL_TOWN_VAL = 3;
    query.bind(ARRIVAL_TOWN_VAL, flight.arrivalTown);
    const int DEPARTURE_DATE_VAL = 4;
    query.bind(DEPARTURE_DATE_VAL, flight.departureDate);
    const int ARRIVAL_DATE_VAL = 5;
    query.bind(ARRIVAL_DATE_VAL, flight.arrivalDate);
    const int ECO_VAL = 6;
    query.bind(ECO_VAL, flight.economicFee);
    const int BUSY_VAL = 7;
    query.bind(BUSY_VAL, flight.busynessFee);
    const int UUID_VAL = 8;
    query.bind(UUID_VAL, flight.uuid);
    int result = 0;
    try {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info("Flight " + flight.number + " has been updated successfully");
        }
    } catch (const SQLite::Exception& e) {
        get_logger().error("Failed to updated flight " + flight.number + " : " + e.what());
    }
    return result;
}

int FlightDataAccess::delete_flight(const std::string& flight_uuid)
{
    SQLite::Statement query(m_db, "DELETE FROM Flights WHERE uuid = ?");
    query.bind(1, flight_uuid);
    int result = 0;
    try {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info("Flight id " + flight_uuid + " has been deleted");
        }
    } catch (const SQLite::Exception& e) {
        get_logger().error("Failed to delete flight " + flight_uuid + " : " + e.what());
    }
    return result;
}

std::optional<Flight> FlightDataAccess::get_flight(const std::string& flight_uuid)
{
    SQLite::Statement query(m_db, "SELECT id, companyId, uuid, codeIcao, "
        "fligthNumber, departureTown, arrivalTown, departureDate, arrivalDate, "
        "economicPlaceFee, busynessPlaceFee, currency FROM Flights WHERE uuid = ?");
    query.bind(1, flight_uuid);
    if (query.executeStep()) {
        return Flight{
            .id = query.getColumn("id").getUInt(),
            .companyId = query.getColumn("companyId").getUInt(),
            .uuid = query.getColumn("uuid").getString(),
            .codeIcao = query.getColumn("codeIcao").getString(),
            .number = query.getColumn("flightNumber").getString(),
            .departureTown = query.getColumn("departureTown").getString(),
            .arrivalTown = query.getColumn("arrivalTown").getString(),
            .departureDate = query.getColumn("departureDate").getString(),
            .arrivalDate = query.getColumn("arrivalDate").getString(),
            .economicFee = static_cast<float>(query.getColumn("economicPlaceFee").getDouble()),
            .busynessFee = static_cast<float>(query.getColumn("busynessPlaceFee").getDouble()),
            .currency = query.getColumn("currency").getString()
        };
    }
    return std::nullopt;
}

std::vector<Flight> FlightDataAccess::list_flights()
{
    SQLite::Statement query(m_db, "SELECT id, companyId, uuid, codeIcao, "
        "fligthNumber, departureTown, arrivalTown, departureDate, arrivalDate, "
        "economicPlaceFee, busynessPlaceFee, currency FROM Flights");
    std::vector<Flight> flights;
    while (query.executeStep()) {
        flights.push_back(
            Flight {
                .id = query.getColumn("id").getUInt(),
                .companyId = query.getColumn("companyId").getUInt(),
                .uuid = query.getColumn("uuid").getString(),
                .codeIcao = query.getColumn("codeIcao").getString(),
                .number = query.getColumn("flightNumber").getString(),
                .departureTown = query.getColumn("departureTown").getString(),
                .arrivalTown = query.getColumn("arrivalTown").getString(),
                .departureDate = query.getColumn("departureDate").getString(),
                .arrivalDate = query.getColumn("arrivalDate").getString(),
                .economicFee = static_cast<float>(query.getColumn("economicPlaceFee").getDouble()),
                .busynessFee = static_cast<float>(query.getColumn("busynessPlaceFee").getDouble()),
                .currency = query.getColumn("currency").getString()
            }
        );
    
    }
    return flights;
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
                "flightNumber TEXT NOT NULL"
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