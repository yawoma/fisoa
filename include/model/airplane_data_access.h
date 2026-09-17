#ifndef AIRPLANE_DATA_ACCESS_H
#define AIRPLANE_DATA_ACCESS_H

#include "SQLiteCpp/Database.h"
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <vector>

namespace fisoa
{
struct Airplane
{
    uint32_t id = 0;
    std::string uuid;
    std::string model;
    uint8_t numberRows; // number of seats rows
    uint8_t numberColumns; // number of seats columns
    uint8_t numberLanes; // number of lanes
    std::string acquisitionDate;
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class AirplaneDataAccess
{
    public:
        AirplaneDataAccess();
        explicit AirplaneDataAccess(SQLite::Database& database);
        ~AirplaneDataAccess() = default;

        int insert_airplane(const Airplane& airplane);
        int update_airplane(const Airplane& airplane);
        int delete_aiplane(const std::string& airplane_uuid);
        Airplane get_airplane(const std::string& airplane_uuid);
        std::vector<Airplane> list_airplanes();

    private:
        // init database and create airplane table 
        void init_database();
        SQLite::Database m_db;
};
} // namespace fisoa
#endif