#ifndef CUSTOMER_DATA_ACCESS_H
#define CUSTOMER_DATA_ACCESS_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdint>
#include <string>
#include <vector>

struct Customer
{
    uint32_t    m_id = 0;
    std::string m_uuid;
    std::string m_firstName;
    std::string m_name;
    std::string m_email;
    std::string m_phone;
    std::string m_address;
    std::string m_gender;
    std::string m_passportId;
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class CustomerDataAccess
{
public:
    CustomerDataAccess();
    ~CustomerDataAccess();

    bool                  insert_customer(const Customer& customer);
    bool                  update_customer(const Customer& customer);
    bool                  delete_customer(const std::string& customer_id);
    Customer              get_customer(const std::string& customer_id);
    std::vector<Customer> list_customers();

private:
    void             init_database();
    // void close_database();
    SQLite::Database m_db;
};

#endif    // CUSTOMER_DATA_ACCESS_H
