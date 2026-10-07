#ifndef CUSTOMER_DATA_ACCESS_H
#define CUSTOMER_DATA_ACCESS_H

#include "SQLiteCpp/Database.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace fisoa
{
struct Customer
{
    uint32_t    id = 0;
    std::string uuid;
    std::string firstName;
    std::string name;
    std::string email;
    std::string phone;
    std::string address;
    std::string gender;
    std::string passportId;
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class CustomerDataAccess
{
public:
    CustomerDataAccess();
    explicit CustomerDataAccess(SQLite::Database& database);
    ~CustomerDataAccess() = default;

    int                     insert_customer(const Customer& customer);
    int                     update_customer(const Customer& customer);
    int                     delete_customer(const std::string& customer_uuid);
    std::optional<Customer> get_customer(const std::string& customer_uuid);
    std::vector<Customer>   list_customers();

    void                    init_database(SQLite::Database& database);
    int                     create_table();

private:
    // void close_database();
    SQLite::Database m_db;
};
}    // namespace fisoa

#endif    // CUSTOMER_DATA_ACCESS_H
