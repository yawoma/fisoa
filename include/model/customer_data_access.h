#ifndef CUSTOMER_DATA_ACCESS_H
#define CUSTOMER_DATA_ACCESS_H

#include "SQLiteCpp/Database.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdint>
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
    explicit CustomerDataAccess();
    explicit CustomerDataAccess(SQLite::Database& database);
    ~CustomerDataAccess();

    int                  insert_customer(const Customer& customer);
    int                  update_customer(const Customer& customer);
    int                  delete_customer(const std::string& customer_uuid);
    Customer              get_customer(const std::string& customer_uuid);
    std::vector<Customer> list_customers();

private:
    void             init_database();
    // void close_database();
    SQLite::Database m_db;
};
} // namespace fisoa

#endif    // CUSTOMER_DATA_ACCESS_H