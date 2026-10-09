#include "model/company_data_access.h"

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Exception.h"
#include "SQLiteCpp/Statement.h"
#include "utils/database.h"
#include "utils/logger.h"

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace fisoa
{
CompanyDataAccess::CompanyDataAccess(): m_db("") {}

CompanyDataAccess::CompanyDataAccess(SQLite::Database& database)
    : m_db(std::move(database))
{
}

int CompanyDataAccess::insert_company(const Company& company)
{
    SQLite::Statement query(
        m_db, "INSERT INTO Companies (name, codeIcao, codeIata, callSign)"
              " VALUES (?, ?, ?, ?)");
    query.bind(1, company.name);
    query.bind(2, company.codeIcao);
    query.bind(3, company.codeIata);
    query.bind(4, company.callSign);
    int result = 0;
    try
    {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info(
                "Company " + company.codeIcao + " has been inserted successfully");
        }
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error(
            "Failed to insert company " + company.codeIcao + " : " +
            std::string(e.what()));
    }
    return result;
}

int CompanyDataAccess::update_company(const Company& company)
{
    SQLite::Statement query(
        m_db, "UPDATE Companies SET name = ?, codeIcao = ?, codeIata = ?, callSign = ?"
              " WHERE id = ?");
    const int32_t NAME_COL = 1;
    query.bind(NAME_COL, company.name);
    const int32_t ICAO_COL = 2;
    query.bind(ICAO_COL, company.codeIcao);
    const int32_t IATA_COL = 3;
    query.bind(IATA_COL, company.codeIata);
    const int32_t CALLSIGN_COL = 4;
    query.bind(CALLSIGN_COL, company.callSign);
    const int32_t ID_COL = 5;
    query.bind(ID_COL, company.id);
    int result = 0;
    try
    {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info(
                "Company with id = " + std::to_string(company.id) +
                " has been updated succesfully");
        }
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error("Failed to update a company : " + std::string(e.what()));
    }
    return result;
}

int CompanyDataAccess::delete_company(const std::string& code_icao)
{
    SQLite::Statement query(m_db, "DELETE FROM Companies WHERE codeIcao = ?");
    query.bind(1, code_icao);
    int result = 0;
    try
    {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info("Company " + code_icao + " has been deleted successfully");
        }
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error("Failed to delete a company : " + std::string(e.what()));
    }
    return result;
}

std::optional<Company> CompanyDataAccess::get_company(const std::string& code_icao)
{
    SQLite::Statement query(
        m_db, "SELECT id, name, codeIcao, codeIata, callSign"
              " FROM Companies WHERE codeIcao = ?");
    query.bind(1, code_icao);
    Company company{};
    if (query.executeStep())
    {
        return Company{
            .id       = query.getColumn("id").getUInt(),
            .name     = query.getColumn("name").getString(),
            .codeIcao = query.getColumn("codeIcao").getString(),
            .codeIata = query.getColumn("codeIata").getString(),
            .callSign = query.getColumn("callSign").getString()};
    }
    return std::nullopt;
}

std::vector<Company> CompanyDataAccess::list_companies()
{
    SQLite::Statement query(
        m_db, "SELECT id, name, codeIcao, codeIata, callSign"
              " FROM Companies");
    std::vector<Company> companies;
    while (query.executeStep())
    {
        Company company;
        company.id       = query.getColumn("id").getInt();
        company.name     = query.getColumn("name").getString();
        company.codeIcao = query.getColumn("codeIcao").getString();
        company.codeIata = query.getColumn("codeIata").getString();
        company.callSign = query.getColumn("callSign").getString();
        companies.push_back(company);
    }
    return companies;
}

void CompanyDataAccess::init_database(SQLite::Database& database)
{
    m_db = std::move(database);
}

int CompanyDataAccess::create_table()
{
    int result = -1;
    if (m_db.getHandle() != nullptr)
    {
        // Check if the companies table exists, if not create it
        if (!m_db.tableExists("Companies"))
        {
            result = m_db.exec(
                "CREATE TABLE Companies (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT "
                "NOT NULL, codeIcao TEXT NOT NULL UNIQUE, codeIata TEXT, callSign TEXT)");
            get_logger().info("Companies table create successfully.");
        }
        else
        {
            get_logger().info("Companies table found.");
            result = 0;
        }
    }
    get_logger().error("Database have not been connected");
    return result;
}
}    // namespace fisoa
