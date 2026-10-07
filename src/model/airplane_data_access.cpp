#include "model/airplane_data_access.h"

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Exception.h"
#include "SQLiteCpp/Statement.h"
#include "utils/database.h"
#include "utils/logger.h"

#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace fisoa
{
AirplaneDataAccess::AirplaneDataAccess(): m_db("") {}
AirplaneDataAccess::AirplaneDataAccess(SQLite::Database& database)
    : m_db(std::move(database))
{
}

int AirplaneDataAccess::insert_airplane(const Airplane& airplane)
{
    SQLite::Statement query(
        m_db,
        "INSERT INTO Airplanes (uuid, model, numberOfRows, "
        "numberOfColumns, numberOfLanes, acquisitionDate) VALUES (?, ?, ?, ?, ?, ?)");
    const int UUID_COL = 1;
    query.bind(UUID_COL, airplane.uuid);
    const int MODEL_COL = 2;
    query.bind(MODEL_COL, airplane.model);
    const int NBROWS_COL = 3;
    query.bind(NBROWS_COL, airplane.numberRows);
    const int NBCOLS_COL = 4;
    query.bind(NBCOLS_COL, airplane.numberColumns);
    const int NBLANES_COL = 5;
    query.bind(NBLANES_COL, airplane.numberLanes);
    const int ACQUI_COL = 6;
    query.bind(ACQUI_COL, airplane.acquisitionDate);
    int result = 0;
    try
    {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info(
                "Airplane " + airplane.model + " has been inserted successfully");
        }
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error(
            "Failed to insert Airplane " + airplane.model + " : " + e.what());
    }
    return result;
}

int AirplaneDataAccess::update_airplane(const Airplane& airplane)
{
    SQLite::Statement query(
        m_db, "UPDATE Airplanes SET model=?, numberOfRows=?, "
              "numberOfColumns=?, numberOfLanes=?, acquisitionDate=? WHERE uuid=?");
    const int MODEL_COL = 1;
    query.bind(MODEL_COL, airplane.model);
    const int NBROWS_COL = 2;
    query.bind(NBROWS_COL, airplane.numberRows);
    const int NBCOLS_COL = 3;
    query.bind(NBCOLS_COL, airplane.numberColumns);
    const int NBLANES_COL = 4;
    query.bind(NBLANES_COL, airplane.numberLanes);
    const int ACQUI_COL = 5;
    query.bind(ACQUI_COL, airplane.acquisitionDate);
    const int UUID_COL = 6;
    query.bind(UUID_COL, airplane.uuid);
    int result = 0;
    try
    {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info(
                "Airplane " + airplane.model + " has been updated successfully");
        }
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error("Failed to update " + airplane.model + " : " + e.what());
    }
    return result;
}

int AirplaneDataAccess::delete_aiplane(const std::string& airplane_uuid)
{
    SQLite::Statement query(m_db, "DELETE FROM Airplanes WHERE uuid=?");
    query.bind(1, airplane_uuid);
    int result = 0;
    try
    {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info(
                "Airplane with uuid " + airplane_uuid + " has been deleted successfully");
        }
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error(
            "Failed to delete airplane with uuid " + airplane_uuid + " : " + e.what());
    }
    return result;
}

std::optional<Airplane> AirplaneDataAccess::get_airplane(const std::string& airplane_uuid)
{
    SQLite::Statement query(
        m_db, "SELECT id, uuid, model, numberOfRows, numberOfColumns,"
              " numberOfLanes, acquisitionDate FROM Airplanes WHERE uuid=?");
    query.bind(1, airplane_uuid);
    if (query.executeStep())
    {
        return Airplane{
            .id              = query.getColumn("id").getUInt(),
            .uuid            = query.getColumn("uuid").getString(),
            .model           = query.getColumn("model").getString(),
            .numberRows      = query.getColumn("numberOfRows").getUInt(),
            .numberColumns   = query.getColumn("numberOfColumns").getUInt(),
            .numberLanes     = query.getColumn("numberOfLanes").getUInt(),
            .acquisitionDate = query.getColumn("acquisitionDate").getString()};
    }
    return std::nullopt;
}

std::vector<Airplane> AirplaneDataAccess::list_airplanes()
{
    SQLite::Statement query(
        m_db, "SELECT id, uuid, model, numberOfRows, numberOfColumns,"
              " numberOfLanes, acquisitionDate FROM Airplanes");
    std::vector<Airplane> airplanes;
    while (query.executeStep())
    {
        airplanes.push_back(
            Airplane{
                .id              = query.getColumn("id").getUInt(),
                .uuid            = query.getColumn("uuid").getString(),
                .model           = query.getColumn("model").getString(),
                .numberRows      = query.getColumn("numberOfRows").getUInt(),
                .numberColumns   = query.getColumn("numberOfColumns").getUInt(),
                .numberLanes     = query.getColumn("numberOfLanes").getUInt(),
                .acquisitionDate = query.getColumn("acquisitionDate").getString()});
    }
    return airplanes;
}

void AirplaneDataAccess::init_database(SQLite::Database& database)
{
    m_db = std::move(database);
}

int AirplaneDataAccess::create_table()
{
    int result = -1;
    if (m_db.getHandle() != nullptr)
    {
        // Check if the airplanes table exists, if not create it
        if (!m_db.tableExists("Airplanes"))
        {
            result = m_db.exec(
                "CREATE TABLE Airplanes (id INTEGER PRIMARY KEY AUTOINCREMENT, uuid TEXT "
                "NOT NULL UNIQUE, "
                "model TEXT NOT NULL, numberOfRows INTEGER NOT NULL, numberOfColumns "
                "INTEGER NOT NULL, numberOfLanes INTEGER NOT NULL, "
                "acquisitionDate DATE)");
            get_logger().info("Airplanes table created");
        }
        else
        {
            get_logger().info("Airplanes table found");
            result = 0;
        }
        return result;
    }
    get_logger().error("Database have not been connected");
    return result;
}
}    // namespace fisoa
