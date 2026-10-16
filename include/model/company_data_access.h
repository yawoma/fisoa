#ifndef COMPANY_DATA_ACCES
#define COMPANY_DATA_ACCES

#include "SQLiteCpp/Database.h"
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace fisoa
{

struct Company
{
    uint32_t    id = 0;
    std::string name;
    std::string codeIcao;
    std::string codeIata;
    std::string callSign;
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class CompanyDataAccess
{
public:
    explicit CompanyDataAccess(SQLite::Database& database);
    ~CompanyDataAccess()                                       = default;

    CompanyDataAccess(const CompanyDataAccess&)                = delete;
    CompanyDataAccess&     operator=(const CompanyDataAccess&) = delete;

    int                    insert_company(const Company& company);
    int                    update_company(const Company& company);
    int                    delete_company(const std::string& code_icao);
    std::optional<Company> get_company(const std::string& code_icao);
    std::vector<Company>   list_companies();

    int                    create_table();

private:
    SQLite::Database& m_db;
};

}    // namespace fisoa

#endif
