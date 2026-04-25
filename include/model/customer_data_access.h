#ifndef CUSTOMER_DATA_ACCESS_H
#define CUSTOMER_DATA_ACCESS_H

#include <string>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>

class Customer;

//NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class CustomerDataAccess
{
public:
    CustomerDataAccess();
    ~CustomerDataAccess();

    bool insert_customer(const Customer& customer);
    bool update_customer(const Customer& customer);
    bool delete_customer(const std::string& customer_id);
    Customer get_customer(const std::string& customer_id);
    std::vector<Customer> list_customers();

private:
    void init_database();
    void close_database();
    SQLite::Database m_db;
};

#endif // CUSTOMER_DATA_ACCESS_H