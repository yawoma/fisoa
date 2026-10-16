#include "model/flight_data_access.h"

#include "SQLiteCpp/Database.h"
#include "model/company_data_access.h"
#include "utils/database.h"

#include <gtest/gtest.h>
#include <memory>
#include <optional>

namespace fisoa
{
class FlightDataAccessTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create SQLite DataBase in memory
        db = std::make_unique<SQLite::Database>(
            ":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        cdao = std::make_unique<CompanyDataAccess>(*db);
        ASSERT_GE(cdao->create_table(), 0);
        fdao = std::make_unique<FlightDataAccess>(*db);
        ASSERT_GE(fdao->create_table(), 0);
    }

    void TearDown() override
    {
        fdao.reset();
        cdao.reset();
        db.reset();
    }

    // NOLINTBEGIN (misc-non-private-member-variables-in-classes)
    std::unique_ptr<SQLite::Database>  db;
    std::unique_ptr<CompanyDataAccess> cdao;
    std::unique_ptr<FlightDataAccess>  fdao;
    // NOLINTEND
};

TEST_F(FlightDataAccessTest, insert_and_get_flight)
{
    // insert company in companies table because company id is as foreign in flight
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    int company_id = cdao->insert_company(company);
    ASSERT_GT(company_id, 0);

    // insert flight
    Flight flight{
        .uuid          = uuid::generate(),
        .number        = "AB513",
        .departureTown = "Jamaiq",
        .arrivalTown   = "Bigest",
        .departureDate = "2026-10-09 13:30:00",
        .arrivalDate   = "2026-10-10 20:15:00",
        .economicFee   = 2000.80,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .busynessFee   = 2500.67,    // NOLINT
        .currency      = "euros"};
    int flight_id = fdao->insert_flight(flight, company.name);
    EXPECT_GT(flight_id, 0);

    auto result = fdao->get_flight(flight.uuid);

    EXPECT_EQ(result->id, flight_id);
    EXPECT_EQ(result->uuid, flight.uuid);
    EXPECT_EQ(result->companyId, company_id);
    EXPECT_EQ(result->codeIcao, "ABB");
    EXPECT_EQ(result->number, "AB513");
    EXPECT_EQ(result->departureTown, "Jamaiq");
    EXPECT_EQ(result->arrivalTown, "Bigest");
    EXPECT_EQ(result->departureDate, "2026-10-09 13:30:00");
    EXPECT_EQ(result->arrivalDate, "2026-10-10 20:15:00");
    EXPECT_EQ(result->economicFee, 2000.80);
    EXPECT_EQ(result->busynessFee, 2500.67);
    EXPECT_EQ(result->currency, "euros");
}

TEST_F(FlightDataAccessTest, get_non_existing_flight)
{
    EXPECT_EQ(fdao->get_flight("does-not-exist-flight-uuid"), std::nullopt);
}

TEST_F(FlightDataAccessTest, unique_uuid)
{
    // insert company
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    int company_id = cdao->insert_company(company);
    ASSERT_GT(company_id, 0);

    // insert flight
    auto   uuid = uuid::generate();
    Flight flight{
        .uuid          = uuid,
        .number        = "AB513",
        .departureTown = "Jamaiq",
        .arrivalTown   = "Bigest",
        .departureDate = "2026-10-09 13:30:00",
        .arrivalDate   = "2026-10-10 20:15:00",
        .economicFee   = 2000.80,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .busynessFee   = 2500.67,    // NOLINT
        .currency      = "euros"};
    int flight_id = fdao->insert_flight(flight, company.name);
    ASSERT_GT(flight_id, 0);

    // diff flight with same uuid
    Flight flight_second{
        .uuid          = uuid,
        .number        = "AB820",
        .departureTown = "Jamme",
        .arrivalTown   = "Little",
        .departureDate = "2026-10-16 09:00:00",
        .arrivalDate   = "2026-10-17 15:30:50",
        .economicFee   = 1500.65,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .busynessFee   = 2900.99,    // NOLINT
        .currency      = "euros"};
    // Faile to insert
    EXPECT_EQ(fdao->insert_flight(flight_second, company.name), 0);
}

TEST_F(FlightDataAccessTest, update_flight)
{
    // insert company in companies table because company id is as foreign in flight
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    int company_id = cdao->insert_company(company);
    ASSERT_GT(company_id, 0);

    // insert flight
    Flight flight{
        .uuid          = uuid::generate(),
        .number        = "AB513",
        .departureTown = "Jamaiq",
        .arrivalTown   = "Bigest",
        .departureDate = "2026-10-09 13:30:00",
        .arrivalDate   = "2026-10-10 20:15:00",
        .economicFee   = 2000.80,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .busynessFee   = 2500.67,    // NOLINT
        .currency      = "euros"};
    int flight_id = fdao->insert_flight(flight, company.name);
    EXPECT_GT(flight_id, 0);
    auto result           = fdao->get_flight(flight.uuid);
    // update dates and fees
    result->departureDate = "2026-10-16 20:30:30";
    result->arrivalDate   = "2026-10-17 10:45:35";
    result->economicFee   = 1900.55;    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
    result->busynessFee   = 2400.89;    // NOLINT
    EXPECT_GT(fdao->update_flight(*result), 0);
    // check
    auto result_second = fdao->get_flight(result->uuid);
    EXPECT_EQ(result_second->departureDate, "2026-10-16 20:30:30");
    EXPECT_EQ(result_second->arrivalDate, "2026-10-17 10:45:35");
    EXPECT_EQ(result_second->economicFee, 1900.55);
    EXPECT_EQ(result_second->busynessFee, 2400.89);
    EXPECT_EQ(result_second->companyId, company_id);
    EXPECT_EQ(result_second->codeIcao, "ABB");
}

TEST_F(FlightDataAccessTest, update_non_existing_flight)
{
    // insert flight
    Flight flight{
        .uuid          = "does-not-exist-flight",
        .number        = "AB513",
        .departureTown = "Jamaiq",
        .arrivalTown   = "Bigest",
        .departureDate = "2026-10-09 13:30:00",
        .arrivalDate   = "2026-10-10 20:15:00",
        .economicFee   = 2000.80,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .busynessFee   = 2500.67,    // NOLINT
        .currency      = "euros"};
    // failed: result will be null
    EXPECT_EQ(fdao->update_flight(flight), 0);
}

TEST_F(FlightDataAccessTest, delete_flight)
{
    // insert company
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    int company_id = cdao->insert_company(company);
    ASSERT_GT(company_id, 0);

    // insert flight
    Flight flight{
        .uuid          = uuid::generate(),
        .number        = "AB513",
        .departureTown = "Jamaiq",
        .arrivalTown   = "Bigest",
        .departureDate = "2026-10-09 13:30:00",
        .arrivalDate   = "2026-10-10 20:15:00",
        .economicFee   = 2000.80,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .busynessFee   = 2500.67,    // NOLINT
        .currency      = "euros"};
    int flight_id = fdao->insert_flight(flight, company.name);
    // delete flight
    EXPECT_GT(fdao->delete_flight(flight.uuid), 0);
    // verify
    EXPECT_EQ(fdao->get_flight(flight.uuid), std::nullopt);
    // company would not be deleted
    auto result = cdao->get_company(company.codeIcao);
    EXPECT_EQ(result->name, "Air Bambi");    // company name

    // try deleteing non existing flight
    EXPECT_EQ(fdao->delete_flight("does-not-exist-flight-uuid"), 0);
}

TEST_F(FlightDataAccessTest, list_flights)
{
    // insert company
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    ASSERT_GT(cdao->insert_company(company), 0);

    // insert first flight
    Flight flight{
        .uuid          = uuid::generate(),
        .number        = "AB513",
        .departureTown = "Jamaiq",
        .arrivalTown   = "Bigest",
        .departureDate = "2026-10-09 13:30:00",
        .arrivalDate   = "2026-10-10 20:15:00",
        .economicFee   = 2000.80,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .busynessFee   = 2500.67,    // NOLINT
        .currency      = "euros"};
    ASSERT_GT(fdao->insert_flight(flight, company.name), 0);

    // insert second flight
    Flight flight_second{
        .uuid          = uuid::generate(),
        .number        = "AB820",
        .departureTown = "Jamme",
        .arrivalTown   = "Little",
        .departureDate = "2026-10-16 09:00:00",
        .arrivalDate   = "2026-10-17 15:30:50",
        .economicFee   = 1500.65,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .busynessFee   = 2900.99,    // NOLINT
        .currency      = "euros"};
    ASSERT_GT(fdao->insert_flight(flight_second, company.name), 0);

    auto flights = fdao->list_flights();
    EXPECT_EQ(flights.size(), 2U);
    // verify
    EXPECT_EQ(flights[0].number, "AB513");
    EXPECT_EQ(flights[0].codeIcao, "ABB");
    EXPECT_EQ(flights[0].economicFee, 2000.80);

    EXPECT_EQ(flights[1].number, "AB820");
    EXPECT_EQ(flights[1].codeIcao, "ABB");
    EXPECT_EQ(flights[1].economicFee, 1500.65);
}
}    // namespace fisoa
