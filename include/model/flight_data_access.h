#ifndef FLIGHT_DATA_ACCESS_H
#define FLIGHT_DATA_ACCESS_H

#include "SQLiteCpp/Database.h"
#include <optional>
#include <vector>

namespace fisoa {
struct Flight{
    uint32_t id = 0;
    uint32_t companyId = 0;
    std::string uuid;
    std::string codeIcao; //< company code ICAO
    std::string departureTown;
    std::string arrivalTown;
    std::string departureDate;
    std::string arrivalDate;
    float economicFee;
    float busynessFee;
    std::string currency; //< the default one would be euro
};

// NOLINTNEXTLINE (cppcoreguidelines-special-member-functions)
class FlightDataAccess {
    public:
        FlightDataAccess();
        explicit FlightDataAccess(SQLite::Database& database);
        ~FlightDataAccess() = default;

        int insert_flight(const Flight& flight);
        int update_flight(const Flight& flight);
        int delete_flight(const std::string& flight_uuid);
        std::optional<Flight> get_flight(const std::string& flight_uuid);
        std::vector<Flight> list_flights();

        void init_database(SQLite::Database& database);
        int create_table();

    private:
        SQLite::Database m_db;
};
} // namespace fisoa
#endif // FLIGHT_DATA_ACCESS_H