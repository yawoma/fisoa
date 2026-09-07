#include "model/customer_data_access.h"

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Exception.h"
#include "SQLiteCpp/Statement.h"
#include "utils/database.h"
#include "utils/logger.h"

#include <cstdint>
#include <string>

namespace fisoa
{
CustomerDataAccess::CustomerDataAccess(): m_db(SQLite::Database("")) { init_database(); }

CustomerDataAccess::CustomerDataAccess(SQLite::Database &database) : m_db(std::move(database)) {}

CustomerDataAccess::~CustomerDataAccess()
{
    // close_database();
}

int CustomerDataAccess::insert_customer(const Customer& customer)
{
    SQLite::Statement query(m_db, "INSERT INTO Customers (uuid, firstName, name, email, phone, "
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
        get_logger().info("Customer with uuid " + customer.uuid + " inserted successfully");
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error("Failed to insert customer: " + std::string(e.what()));
    }
    return result;
}

int CustomerDataAccess::update_customer(const Customer& customer)
{
    std::string request =
        "UPDATE Customers SET firstName = ?, name = ?, email = ?, phone = ?, address = ?, gender = ?, passportId = ?"
        " WHERE uuid = ?";
    SQLite::Statement query(m_db, request);
    const int32_t FIRSTNAME_COL = 1;
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
        get_logger().info("Customer with uuid " + customer.uuid + " has been updated successfully");
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
        get_logger().info("Customer with uuid " + customer_uuid + " has been deleted successfully");
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error("Failed to delete customer: " + std::string(e.what()));
    }
    return result;
}

Customer CustomerDataAccess::get_customer(const std::string& customer_uuid)
{
    std::string request = "SELECT id, uuid, firstName, name, email, phone, address, "
                          "gender, passportId FROM Customers WHERE uuid = ?";
    SQLite::Statement query(m_db, request);
    query.bind(1, customer_uuid);
    Customer          customer{};
    if (query.executeStep())
    {
        customer.id         = query.getColumn("id").getInt();
        customer.uuid       = query.getColumn("uuid").getString();
        customer.firstName  = query.getColumn("firstName").getString();
        customer.name       = query.getColumn("name").getString();
        customer.email      = query.getColumn("email").getString();
        customer.phone      = query.getColumn("phone").getString();
        customer.address    = query.getColumn("address").getString();
        customer.gender     = query.getColumn("gender").getString();
        customer.passportId = query.getColumn("passportId").getString();

        get_logger().info("Got customer with uuid " + customer_uuid);
    }
    else
    {
        get_logger().warn("Customer with uuid " + customer_uuid + " not found");
    }
    return customer;
}

std::vector<Customer> CustomerDataAccess::list_customers()
{
    std::string request = "SELECT id, uuid, firstName, name, email, phone, address, "
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

void CustomerDataAccess::init_database()
{
    if (create_database_directory())
    {
        std::string db_path = get_database_path();
        m_db = SQLite::Database(db_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        get_logger().info("Database initialized successfully at: " + db_path);
        // Check if the customers table exists, if not create it
        if (!m_db.tableExists("Customers"))
        {
            m_db.exec(
                "CREATE TABLE Customers (id INTEGER PRIMARY KEY AUTOINCREMENT, uuid TEXT NOT NULL UNIQUE, firstName "
                "TEXT NOT NULL, name TEXT NOT NULL, email TEXT NOT NULL, phone TEXT NOT NULL, address TEXT NOT NULL, gender TEXT, "
                "passportId TEXT NOT NULL UNIQUE)");
            get_logger().info("Customers table created successfully.");
        }
        else
        {
            get_logger().info("Customers table found.");
        }
    }
    else
    {
        throw std::runtime_error("Failed to create database directory");
    }
}

// void CustomerDataAccess::close_database()
// {
//     m_db.close();
//     get_logger().info("Database connection closed successfully.");
// }
} // namespace fisoa