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
    static json_t getAllCustomers();
    static json_t getCustomer(/*customer id*/);
    static json_t getCustomer(std::string customerName);
};

#endif    // CUSTOMER_DATA_ACCESS_H
