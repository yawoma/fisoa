#include "model/customer_data_access.h"

#include "utils/database.h"
#include "utils/logger.h"

#include <string>

CustomerDataAccess::CustomerDataAccess(): m_db("") { init_database(); }

CustomerDataAccess::~CustomerDataAccess()
{
    // close_database();
}

bool CustomerDataAccess::insert_customer(const Customer& customer)
{
    std::string request = "INSERT INTO Customers (uuid, firstName, name, email, phone, "
                          "address, gender, passportId) VALUES ('" +
                          customer.m_uuid + "', '" + customer.m_firstName + "', '" +
                          customer.m_name + "', '" + customer.m_email + "', '" +
                          customer.m_phone + "', '" + customer.m_address + "', '" +
                          customer.m_gender + "', '" + customer.m_passportId + "')";
    try
    {
        m_db.exec(request);
        get_logger().info("Customer inserted successfully: " + customer.m_uuid);
    }
    catch (const std::exception& e)
    {
        get_logger().error("Failed to insert customer: " + std::string(e.what()));
        return false;
    }
    return true;
}

bool CustomerDataAccess::update_customer(const Customer& customer)
{
    std::string request =
        "UPDATE Customers SET firstName = '" + customer.m_firstName + "', name = '" +
        customer.m_name + "', email = '" + customer.m_email + "', phone = '" +
        customer.m_phone + "', address = '" + customer.m_address + "', gender = '" +
        customer.m_gender + "', passportId = '" + customer.m_passportId +
        "' WHERE uuid = '" + customer.m_uuid + "'";
    try
    {
        m_db.exec(request);
        get_logger().info("Customer updated successfully: " + customer.m_uuid);
    }
    catch (const std::exception& e)
    {
        get_logger().error("Failed to update customer: " + std::string(e.what()));
        return false;
    }
    return true;
}

bool CustomerDataAccess::delete_customer(const std::string& customer_id)
{
    std::string request = "DELETE FROM Customers WHERE uuid = '" + customer_id + "'";
    try
    {
        m_db.exec(request);
        get_logger().info("Customer deleted successfully: " + customer_id);
    }
    catch (const std::exception& e)
    {
        get_logger().error("Failed to delete customer: " + std::string(e.what()));
        return false;
    }
    return true;
}

Customer CustomerDataAccess::get_customer(const std::string& customer_id)
{
    std::string request = "SELECT id, uuid, firstName, name, email, phone, address, "
                          "gender, passportId FROM Customers WHERE uuid = '" +
                          customer_id + "'";
    SQLite::Statement query(m_db, request);
    Customer          customer;
    if (query.executeStep())
    {
        customer.m_id         = query.getColumn(query.getIndex("id")).getInt();
        customer.m_uuid       = query.getColumn(query.getIndex("uuid")).getString();
        customer.m_firstName  = query.getColumn(query.getIndex("firstName")).getString();
        customer.m_name       = query.getColumn(query.getIndex("name")).getString();
        customer.m_email      = query.getColumn(query.getIndex("email")).getString();
        customer.m_phone      = query.getColumn(query.getIndex("phone")).getString();
        customer.m_address    = query.getColumn(query.getIndex("address")).getString();
        customer.m_gender     = query.getColumn(query.getIndex("gender")).getString();
        customer.m_passportId = query.getColumn(query.getIndex("passportId")).getString();
    }
    else
    {
        get_logger().warn("Customer not found: " + customer_id);
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
        customer.m_id         = query.getColumn(query.getIndex("id")).getInt();
        customer.m_uuid       = query.getColumn(query.getIndex("uuid")).getString();
        customer.m_firstName  = query.getColumn(query.getIndex("firstName")).getString();
        customer.m_name       = query.getColumn(query.getIndex("name")).getString();
        customer.m_email      = query.getColumn(query.getIndex("email")).getString();
        customer.m_phone      = query.getColumn(query.getIndex("phone")).getString();
        customer.m_address    = query.getColumn(query.getIndex("address")).getString();
        customer.m_gender     = query.getColumn(query.getIndex("gender")).getString();
        customer.m_passportId = query.getColumn(query.getIndex("passportId")).getString();
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
                "CREATE TABLE Customers (id INTEGER PRIMARY KEY, uuid TEXT, firstName "
                "TEXT, name TEXT, email TEXT, phone TEXT, address TEXT, gender TEXT, "
                "passportId TEXT)");
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
