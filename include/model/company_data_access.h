#ifndef COMPANY_DATA_ACCES
#define COMPANY_DATA_ACCES

#include "SQLiteCpp/Database.h"
#include <cstdint>
#include <string>
#include <vector>

namespace fisoa {

struct Company{
 uint32_t id = 0;
 std::string name;
 std::string codeIcao;
 std::string codeIata;
 std::string callSign;
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class CompanyDataAccess
{
    public:
        CompanyDataAccess();
        explicit CompanyDataAccess(SQLite::Database& database);
        ~CompanyDataAccess() = default;

        int insert_company(const Company& company);
        int update_company(const Company& company);
        int delete_company(const std::string& code_icao);
        Company get_company(const std::string& code_icao);
        std::vector<Company> list_companies();
    
    private:
        void init_database();
        SQLite::Database m_db;
};

} // namaspace fisoa

#endif