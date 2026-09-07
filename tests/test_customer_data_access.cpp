#include "model/customer_data_access.h"

#include "utils/database.h"
#include <gtest/gtest.h>
#include <SQLiteCpp/Statement.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/Database.h>
#include <memory>
#include <string>


namespace fisoa {

class CustomerDataAccessTest : public ::testing::Test {    
    protected:
        void SetUp() override {
            // Create a SQLite DB in memory
            // SQLite::Database sqliteDb = database_builder(":memory:"); // already tested in SQLiteCpp
            db = std::make_unique<SQLite::Database>(":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            // Create schema
            // const char* schema = ;
            int val = db->exec(
                "CREATE TABLE Customers ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "uuid TEXT NOT NULL UNIQUE,"
                "firstName TEXT NOT NULL,"
                "name TEXT NOT NULL,"
                "email TEXT NOT NULL,"
                "phone TEXT NOT NULL,"
                "address TEXT NOT NULL,"
                "gender TEXT,"
                "passportId TEXT NOT NULL UNIQUE"
                ")"
            );
            EXPECT_EQ(val, 0);
            ASSERT_TRUE(db->tableExists("Customers"));

            cdao = std::make_unique<CustomerDataAccess>(*db);
        }

        void TearDown() override {
            cdao.reset();
            db.reset();
        }

        // NOLINTNEXTLINE (non-private-member-variables-in-classes)
        std::unique_ptr<SQLite::Database> db;
        // NOLINTNEXTLINE (misc-non-private-member-variables-in-classes)
        std::unique_ptr<CustomerDataAccess> cdao;
};

TEST_F(CustomerDataAccessTest, insert_and_get_customer) {
    std::string uuid = fisoa::uuid::generate();
    Customer customer{
        .uuid=uuid,
        .firstName="Bob", 
        .name="Kitani", 
        .email="bob@test.com", 
        .phone="06 22 56 32 58", 
        .address="1 rue bobi 35000 Jamaiq", 
        .gender="", .passportId="AA0000000"};
    int customer_id = cdao->insert_customer(customer);
    EXPECT_GT(customer_id, 0);

    auto result = cdao->get_customer(uuid);

    EXPECT_EQ(result.id, customer_id);
    EXPECT_EQ(result.uuid, uuid);
    EXPECT_EQ(result.firstName, "Bob");
    EXPECT_EQ(result.name, "Kitani");
    EXPECT_EQ(result.email, "bob@test.com");
    EXPECT_EQ(result.phone, "06 22 56 32 58");
    EXPECT_EQ(result.address, "1 rue bobi 35000 Jamaiq");
    EXPECT_EQ(result.gender, "");
    EXPECT_EQ(result.passportId, "AA0000000");
}  

TEST_F(CustomerDataAccessTest, get_non_exist_customer)
{
    auto result = cdao->get_customer("5759456d-0ad2-4a3b-ba06-5bcac4f33481");
    EXPECT_EQ(result.id, 0);
}

TEST_F(CustomerDataAccessTest, same_uuid_failed)
{
    std::string uuid = fisoa::uuid::generate();
    Customer customer{
        .uuid=uuid,
        .firstName="Bob", 
        .name="Kitani", 
        .email="bob@test.com", 
        .phone="06 22 56 32 58", 
        .address="1 rue bobi 35000 Jamaiq", 
        .gender="", .passportId="AA0000000"};
    int customer_id = cdao->insert_customer(customer);

    // asign the same uuid to another customer
    Customer customer2{
        .uuid=uuid,
        .firstName="Boby", 
        .name="Kotano", 
        .email="boby@test.com", 
        .phone="06 22 50 32 58", 
        .address="1 rue boby 35000 Jama", 
        .gender="", .passportId="BB0001000"};
    EXPECT_EQ(cdao->insert_customer(customer), 0);
}

TEST_F(CustomerDataAccessTest, same_passport_id_failed)
{
    std::string uuid = fisoa::uuid::generate();
    Customer customer{
        .uuid=uuid,
        .firstName="Bob", 
        .name="Kitani", 
        .email="bob@test.com", 
        .phone="06 22 56 32 58", 
        .address="1 rue bobi 35000 Jamaiq", 
        .gender="", .passportId="AA0000000"};
    int customer_id = cdao->insert_customer(customer);

    // assign the same passport id to another customer
    uuid = fisoa::uuid::generate();
    Customer customer2{
        .uuid=uuid,
        .firstName="Boby", 
        .name="Kotano", 
        .email="boby@test.com", 
        .phone="06 22 50 32 58", 
        .address="1 rue boby 35000 Jama", 
        .gender="", .passportId="AA0000000"};
    EXPECT_EQ(cdao->insert_customer(customer2), 0);
}

TEST_F(CustomerDataAccessTest, update_customer)
{
    std::string uuid = fisoa::uuid::generate();
    Customer customer{
        .uuid=uuid,
        .firstName="Bob", 
        .name="Kitani", 
        .email="bob@test.com", 
        .phone="06 22 56 32 58", 
        .address="1 rue bobi 35000 Jamaiq", 
        .gender="", .passportId="AA0000000"};
    int customer_id = cdao->insert_customer(customer);

    // change customer phone number
    customer.phone = "07 22 56 47 67";
    EXPECT_GT(cdao->update_customer(customer), 0);
    // verify
    auto result = cdao->get_customer(uuid);
    EXPECT_EQ(result.phone, "07 22 56 47 67");
}

TEST_F(CustomerDataAccessTest, update_non_exist_customer)
{
    // create customer with fake uuid and do not insert it
    Customer customer{
        .uuid="uuid-does-not-exist",
        .firstName="Bob", 
        .name="Kitani", 
        .email="bob@test.com", 
        .phone="06 22 56 32 58", 
        .address="1 rue bobi 35000 Jamaiq", 
        .gender="", .passportId="CC0000000"};
    // try to update non existing customer
    EXPECT_EQ(cdao->update_customer(customer), 0);
}

TEST_F(CustomerDataAccessTest, delete_customer)
{
    std::string uuid = fisoa::uuid::generate();
    Customer customer{
        .uuid=uuid,
        .firstName="Bob", 
        .name="Kitani", 
        .email="bob@test.com", 
        .phone="06 22 56 32 58", 
        .address="1 rue bobi 35000 Jamaiq", 
        .gender="", .passportId="AA0000000"};
    int customer_id = cdao->insert_customer(customer);

    // delete the inserted customer
    EXPECT_GT(cdao->delete_customer(customer.uuid), 0);
    // verify  -- 0 -> default value of id for customer struct object
    EXPECT_EQ(cdao->get_customer(customer.uuid).id, 0); 
}

TEST_F(CustomerDataAccessTest, delete_non_exist_customer)
{
    // create customer with fake uuid and do not insert it
    Customer customer{
        .uuid="uuid-does-not-exist-again",
        .firstName="Bob", 
        .name="Kitani", 
        .email="bob@test.com", 
        .phone="06 22 56 32 58", 
        .address="1 rue bobi 35000 Jamaiq", 
        .gender="", .passportId="CC0000000"};
    // try to delete non existing customer
    EXPECT_EQ(cdao->delete_customer(customer.uuid), 0);
}

TEST_F(CustomerDataAccessTest, all_customers)
{
    // first customer
    std::string uuid = fisoa::uuid::generate();
    Customer customer{
        .uuid=uuid,
        .firstName="Bob", 
        .name="Kitani", 
        .email="bob@test.com", 
        .phone="06 22 56 32 58", 
        .address="1 rue bobi 35000 Jamaiq", 
        .gender="", .passportId="AA0000000"};
    EXPECT_GT(cdao->insert_customer(customer), 0);

    // second customer
    uuid = fisoa::uuid::generate();
    Customer customer2{
        .uuid=uuid,
        .firstName="Boby", 
        .name="Panto", 
        .email="boby@test.com", 
        .phone="07 02 50 32 58", 
        .address="1 rue boby 35000 Jama", 
        .gender="", .passportId="BB0000000"};
    EXPECT_GT(cdao->insert_customer(customer2), 0);

    auto customers = cdao->list_customers();
    ASSERT_EQ(customers.size(), 2U);
    // verify
    EXPECT_EQ(customers[0].name, "Kitani");
    EXPECT_EQ(customers[1].name, "Panto");
}

TEST_F(CustomerDataAccessTest, fresh_db_per_test)
{
    /* This tests is to check tests isolating in :memory: db */

    // table is empty: alls inserts from other tests are gone ->
    // each test get brand new in :memory: tests
    EXPECT_EQ(cdao->list_customers().size(), 0U);
}
} // namespase fisoa
