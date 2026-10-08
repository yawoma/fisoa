#include "model/customer_data_access.h"

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
CustomerDataAccess::CustomerDataAccess(): m_db(SQLite::Database("")) {}

CustomerDataAccess::CustomerDataAccess(SQLite::Database& database)
    : m_db(std::move(database))
{
}

int CustomerDataAccess::insert_customer(const Customer& customer)
{
    SQLite::Statement query(
        m_db, "INSERT INTO Customers (uuid, firstName, name, email, phone, "
              "address, gender, passportId) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    const int32_t UUID_COL = 1;
    query.bind(UUID_COL, customer.uuid);
    const int32_t FIRST_NAME_COL = 2;
    query.bind(FIRST_NAME_COL, customer.firstName);
    const int32_t NAME_COL = 3;
    query.bind(NAME_COL, customer.name);
    const int32_t EMAIL_COL = 4;
    query.bind(EMAIL_COL, customer.email);
    const int32_t PHONE_COL = 5;
    query.bind(PHONE_COL, customer.phone);
    const int32_t ADDRESS_COL = 6;
    query.bind(ADDRESS_COL, customer.address);
    const int32_t GENDER_COL = 7;
    query.bind(GENDER_COL, customer.gender);
    const int32_t PASSPORT_COL = 8;
    query.bind(PASSPORT_COL, customer.passportId);
    int result = 0;
    try
    {
        result = query.exec();
        get_logger().info(
            "Customer with uuid " + customer.uuid + " inserted successfully");
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error("Failed to insert customer: " + std::string(e.what()));
    }
    return result;
}

int CustomerDataAccess::update_customer(const Customer& customer)
{
    const char* request = "UPDATE Customers SET firstName = ?, name = ?, email = ?, "
                          "phone = ?, address = ?, gender = ?, passportId = ?"
                          " WHERE uuid = ?";
    SQLite::Statement query(m_db, request);
    const int32_t     FIRSTNAME_COL = 1;
    query.bind(FIRSTNAME_COL, customer.firstName);
    const int32_t NAME_COL = 2;
    query.bind(NAME_COL, customer.name);
    const int32_t EMAIL_COL = 3;
    query.bind(EMAIL_COL, customer.email);
    const int32_t PHONE_COL = 4;
    query.bind(PHONE_COL, customer.phone);
    const int32_t ADDRESS_COL = 5;
    query.bind(ADDRESS_COL, customer.address);
    const int32_t GENDER_COL = 6;
    query.bind(GENDER_COL, customer.gender);
    const int32_t PASSPORT_COL = 7;
    query.bind(PASSPORT_COL, customer.passportId);
    const int32_t UUID_COL = 8;
    query.bind(UUID_COL, customer.uuid);
    int result = 0;
    try
    {
        result = query.exec();
        get_logger().info(
            "Customer with uuid " + customer.uuid + " has been updated successfully");
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error("Failed to update customer: " + std::string(e.what()));
    }
    return result;
}

int CustomerDataAccess::delete_customer(const std::string& customer_uuid)
{
    SQLite::Statement query(m_db, "DELETE FROM Customers WHERE uuid = ?");
    query.bind(1, customer_uuid);
    int result = 0;
    try
    {
        result = query.exec();
        get_logger().info(
            "Customer with uuid " + customer_uuid + " has been deleted successfully");
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error("Failed to delete customer: " + std::string(e.what()));
    }
    return result;
}

std::optional<Customer> CustomerDataAccess::get_customer(const std::string& customer_uuid)
{
    const char* request = "SELECT id, uuid, firstName, name, email, phone, address, "
                          "gender, passportId FROM Customers WHERE uuid = ?";
    SQLite::Statement query(m_db, request);
    query.bind(1, customer_uuid);
    if (query.executeStep())
    {
        return Customer{
            .id         = query.getColumn("id").getUInt(),
            .uuid       = query.getColumn("uuid").getString(),
            .firstName  = query.getColumn("firstName").getString(),
            .name       = query.getColumn("name").getString(),
            .email      = query.getColumn("email").getString(),
            .phone      = query.getColumn("phone").getString(),
            .address    = query.getColumn("address").getString(),
            .gender     = query.getColumn("gender").getString(),
            .passportId = query.getColumn("passportId").getString()};
    }

    return std::nullopt;
}

std::vector<Customer> CustomerDataAccess::list_customers()
{
    const char* request = "SELECT id, uuid, firstName, name, email, phone, address, "
                          "gender, passportId FROM Customers";
    SQLite::Statement     query(m_db, request);
    std::vector<Customer> customers;
    while (query.executeStep())
    {
        Customer customer;
        customer.id         = query.getColumn("id").getInt();
        customer.uuid       = query.getColumn("uuid").getString();
        customer.firstName  = query.getColumn("firstName").getString();
        customer.name       = query.getColumn("name").getString();
        customer.email      = query.getColumn("email").getString();
        customer.phone      = query.getColumn("phone").getString();
        customer.address    = query.getColumn("address").getString();
        customer.gender     = query.getColumn("gender").getString();
        customer.passportId = query.getColumn("passportId").getString();
        customers.push_back(customer);
    }
    return customers;
}

void CustomerDataAccess::init_database(SQLite::Database& database)
{
    m_db = std::move(database);
}

int CustomerDataAccess::create_table()
{
    int result = -1;
    if (m_db.getHandle() != nullptr)
    {
        // Check if the customers table exists, if not create it
        if (!m_db.tableExists("Customers"))
        {
            result = m_db.exec(
                "CREATE TABLE Customers (id INTEGER PRIMARY KEY AUTOINCREMENT, uuid TEXT "
                "NOT NULL UNIQUE, firstName "
                "TEXT NOT NULL, name TEXT NOT NULL, email TEXT NOT NULL, phone TEXT NOT "
                "NULL, address TEXT NOT NULL, gender TEXT, "
                "passportId TEXT NOT NULL UNIQUE)");
            get_logger().info("Customers table created successfully.");
        }
        else
        {
            get_logger().info("Customers table found.");
            result = 0;
        }
    }
    get_logger().error("Database have not been connected");
    return result;
}

// void CustomerDataAccess::close_database()
// {
//     m_db.close();
//     get_logger().info("Database connection closed successfully.");
// }
}    // namespace fisoa
