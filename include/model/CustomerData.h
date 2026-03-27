#ifndef CUSTOMER_DATA_ACCESS_H
#define CUSTOMER_DATA_ACCESS_H

#include "nlohmann/json_fwd.hpp"
#include <nlohmann/json.hpp>
#include <string>

using json_t = nlohmann::json;

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class CustomerData
{
    public:
    // Get
    static json_t getAllCustomers();
    static json_t getCustomer(int customerId); //< with Id
    static json_t getCustomer(const std::string& firstName, const std::string& name);
    // Post: add
    static void postCustomer(const json_t& customer);
    // Put: update
    static void putCustomer(const json_t& customer);
    // Delete
    static void deleteCustomer(const json_t& customer);
};

#endif    // CUSTOMER_DATA_ACCESS_H
