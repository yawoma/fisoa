#ifndef AIRPLANE_DATA_ACCESS_H
#define AIRPLANE_DATA_ACCESS_H

#include "SQLiteCpp/Database.h"
#include <cstdint>
#include <optional>
#include <string>
#include <sys/types.h>
#include <vector>

namespace fisoa
{
struct Airplane
{
    uint32_t    id = 0;
    std::string uuid;
    std::string model;
    uint32_t    numberRows    = 0;    // number of seats rows
    uint32_t    numberColumns = 0;    // number of seats columns
    uint32_t    numberLanes   = 0;    // number of lanes
    std::string acquisitionDate;
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class AirplaneDataAccess
{
public:
    AirplaneDataAccess();
    explicit AirplaneDataAccess(SQLite::Database& database);
    ~AirplaneDataAccess() = default;

    int                     insert_airplane(const Airplane& airplane);
    int                     update_airplane(const Airplane& airplane);
    int                     delete_aiplane(const std::string& airplane_uuid);
    std::optional<Airplane> get_airplane(const std::string& airplane_uuid);
    std::vector<Airplane>   list_airplanes();

    void                    init_database(SQLite::Database& database);
    int                     create_table();

private:
    SQLite::Database m_db;
};
}    // namespace fisoa
#endif    // AIRPLANE_DATA_ACCESS_H
