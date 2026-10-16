#include "SQLiteCpp/Database.h"
#include "model/booking_data_access.h"

#include "model/company_data_access.h"
#include "model/customer_data_access.h"
#include "model/flight_data_access.h"
#include "utils/database.h"

#include <cstdint>
#include <gtest/gtest.h>
#include <memory>
#include <optional>

namespace fisoa {
class BookingDataAccessTest : public ::testing::Test {
    protected:
        void SetUp() override
        {
            // Create SQLite DataBase in memory
            db = std::make_unique<SQLite::Database>(
                ":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            cudao = std::make_unique<CustomerDataAccess>(*db);
            ASSERT_GE(cudao->create_table(), 0);
            codao = std::make_unique<CompanyDataAccess>(*db);
            ASSERT_GE(codao->create_table(), 0);
            fdao = std::make_unique<FlightDataAccess>(*db);
            ASSERT_GE(fdao->create_table(), 0);
            bdao = std::make_unique<BookingDataAccess>(*db);
            ASSERT_GE(bdao->create_table(), 0);
        }

        void TearDown() override
        {
            bdao.reset();
            fdao.reset();
            codao.reset();
            cudao.reset();
            db.reset();
        }

        // NOLINTBEGIN (misc-non-private-member-variables-in-classes)
        std::unique_ptr<SQLite::Database> db;
        std::unique_ptr<CustomerDataAccess> cudao;
        std::unique_ptr<CompanyDataAccess> codao;
        std::unique_ptr<FlightDataAccess> fdao;
        std::unique_ptr<BookingDataAccess> bdao;
        // NOLINTEND
};

TEST_F(BookingDataAccessTest, insert_and_get_booking)
{
    // insert a customer in customers table
    Customer    customer{
           .uuid       = uuid::generate(),
           .firstName  = "Bob",
           .name       = "Kitani",
           .email      = "bob@test.com",
           .phone      = "06 22 56 32 58",
           .address    = "1 rue bobi 35000 Jamaiq",
           .gender     = "",
           .passportId = "AA0000000"};
    int customer_id = cudao->insert_customer(customer);
    ASSERT_GT(customer_id, 0);

    // insert company into Companies' table
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    int company_id = codao->insert_company(company);
    ASSERT_GT(company_id, 0);

    // insert flight in Flights table
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
    ASSERT_GT(flight_id, 0);

    // insert booking
    Booking booking{
        .reservationId = get_timestamp_millisecond(),
        .date = "2026-10-16",
        .isCanceled = false,
        .seatNumber = 10 // NOLINT
    };
    int booking_id = bdao->insert_booking(booking, customer_id, flight_id);
    EXPECT_GT(booking_id, 0);

    auto result = bdao->get_booking(booking.reservationId);

    EXPECT_EQ(result->id, booking_id);
    EXPECT_EQ(result->customerUUID, customer.uuid);
    EXPECT_EQ(result->flightUUID, flight.uuid);
    EXPECT_EQ(result->reservationId, booking.reservationId);
    EXPECT_EQ(result->date, "2026-10-16");
    EXPECT_FALSE(result->isCanceled);
    EXPECT_EQ(result->seatNumber, 10);
}

TEST_F(BookingDataAccessTest, get_non_existing_booking)
{
    int64_t does_not_exist_id = 125469873; // NOLINT
    EXPECT_EQ(bdao->get_booking(does_not_exist_id), std::nullopt);
}

TEST_F(BookingDataAccessTest, unique_reservation_id)
{
    // insert a customer in customers table
    Customer    customer{
           .uuid       = uuid::generate(),
           .firstName  = "Bob",
           .name       = "Kitani",
           .email      = "bob@test.com",
           .phone      = "06 22 56 32 58",
           .address    = "1 rue bobi 35000 Jamaiq",
           .gender     = "",
           .passportId = "AA0000000"};
    int customer_id = cudao->insert_customer(customer);
    ASSERT_GT(customer_id, 0);

    // insert company into Companies' table
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    int company_id = codao->insert_company(company);
    ASSERT_GT(company_id, 0);

    // insert flight in Flights table
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
    ASSERT_GT(flight_id, 0);

    // insert first booking
    int64_t reservation_id = get_timestamp_millisecond();
    Booking booking{
        .reservationId = reservation_id,
        .date = "2026-10-16",
        .isCanceled = false,
        .seatNumber = 10 // NOLINT
    };
    ASSERT_GT(bdao->insert_booking(booking, customer_id, flight_id), 0);

    // insert second different booking with same id
    Booking booking_second{
        .reservationId = reservation_id,
        .date = "2026-10-17",
        .isCanceled = false,
        .seatNumber = 15 // NOLINT
    };
    EXPECT_EQ(bdao->insert_booking(booking_second, customer_id, flight_id), 0);
}

TEST_F(BookingDataAccessTest, update_booking)
{
    // insert a customer in customers table
    Customer    customer{
           .uuid       = uuid::generate(),
           .firstName  = "Bob",
           .name       = "Kitani",
           .email      = "bob@test.com",
           .phone      = "06 22 56 32 58",
           .address    = "1 rue bobi 35000 Jamaiq",
           .gender     = "",
           .passportId = "AA0000000"};
    int customer_id = cudao->insert_customer(customer);
    ASSERT_GT(customer_id, 0);

    // insert company into Companies' table
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    int company_id = codao->insert_company(company);
    ASSERT_GT(company_id, 0);

    // insert flight in Flights table
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
    ASSERT_GT(flight_id, 0);

    // insert booking
    Booking booking{
        .reservationId = get_timestamp_millisecond(),
        .date = "2026-10-16",
        .isCanceled = false,
        .seatNumber = 10 // NOLINT
    };
    int booking_id = bdao->insert_booking(booking, customer_id, flight_id);
    ASSERT_GT(booking_id, 0);
    
    auto result = bdao->get_booking(booking.reservationId);

    // update booking
    auto reservation_id = get_timestamp_millisecond();
    result->reservationId = reservation_id;
    result->date = "2026-10-17";
    result->seatNumber = 15; //NOLINT
    EXPECT_GT(bdao->update_booking(*result), 0);
    // check
    auto result_second = bdao->get_booking(result->reservationId);
    EXPECT_EQ(result_second->id, booking_id);
    EXPECT_EQ(result_second->customerUUID, customer.uuid);
    EXPECT_EQ(result_second->flightUUID, flight.uuid);
    EXPECT_EQ(result_second->reservationId, reservation_id);
    EXPECT_EQ(result_second->date, "2026-10-17");
    EXPECT_FALSE(result_second->isCanceled);
    EXPECT_EQ(result_second->seatNumber, 15);
}

TEST_F(BookingDataAccessTest, update_non_existing_booking)
{
    const auto does_not_exist_id = 12356487;
    Booking booking{
        .reservationId = does_not_exist_id,
        .date = "2026-10-16",
        .isCanceled = false,
        .seatNumber = 10 // NOLINT
    };
    EXPECT_EQ(bdao->update_booking(booking), 0);
}

TEST_F(BookingDataAccessTest, delete_booking)
{
    // insert a customer in customers table
    Customer    customer{
           .uuid       = uuid::generate(),
           .firstName  = "Bob",
           .name       = "Kitani",
           .email      = "bob@test.com",
           .phone      = "06 22 56 32 58",
           .address    = "1 rue bobi 35000 Jamaiq",
           .gender     = "",
           .passportId = "AA0000000"};
    int customer_id = cudao->insert_customer(customer);
    ASSERT_GT(customer_id, 0);

    // insert company into Companies' table
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    int company_id = codao->insert_company(company);
    ASSERT_GT(company_id, 0);

    // insert flight in Flights table
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
    ASSERT_GT(flight_id, 0);

    /* to delete booking it would be have one year old minimum */
    /* so insert booking with timestamp relative to last year about impl date (2025-10-16 00:00:00) */
    const int64_t reservation_id = 1760565600;
    Booking booking{
        .reservationId = reservation_id,
        .date = "2025-10-16",
        .isCanceled = false,
        .seatNumber = 10 // NOLINT
    };
    int booking_id = bdao->insert_booking(booking, customer_id, flight_id);
    ASSERT_GT(booking_id, 0);
    // delete booking
    EXPECT_GT(bdao->delete_booking(reservation_id), 0);
    // check
    EXPECT_EQ(bdao->get_booking(reservation_id), std::nullopt);
}

TEST_F(BookingDataAccessTest, delete_booking_failed)
{
    // insert a customer in customers table
    Customer    customer{
           .uuid       = uuid::generate(),
           .firstName  = "Bob",
           .name       = "Kitani",
           .email      = "bob@test.com",
           .phone      = "06 22 56 32 58",
           .address    = "1 rue bobi 35000 Jamaiq",
           .gender     = "",
           .passportId = "AA0000000"};
    int customer_id = cudao->insert_customer(customer);
    ASSERT_GT(customer_id, 0);

    // insert company into Companies' table
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    int company_id = codao->insert_company(company);
    ASSERT_GT(company_id, 0);

    // insert flight in Flights table
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
    ASSERT_GT(flight_id, 0);

    /* to delete booking it would be have one year old minimum */
    /* lets get timestamp now -> it will not pass one year */
    Booking booking{
        .reservationId = get_timestamp_millisecond(),
        .date = "2025-10-16",
        .isCanceled = false,
        .seatNumber = 10 // NOLINT
    };
    int booking_id = bdao->insert_booking(booking, customer_id, flight_id);
    ASSERT_GT(booking_id, 0);
    // failed to delete: result will be nill
    EXPECT_EQ(bdao->delete_booking(booking.reservationId), 0);

    // trying deleting non exist booking: failed
    EXPECT_EQ(bdao->delete_booking(123456), 0);
}

TEST_F(BookingDataAccessTest, cancel_booking)
{
    // insert a customer in customers table
    Customer    customer{
           .uuid       = uuid::generate(),
           .firstName  = "Bob",
           .name       = "Kitani",
           .email      = "bob@test.com",
           .phone      = "06 22 56 32 58",
           .address    = "1 rue bobi 35000 Jamaiq",
           .gender     = "",
           .passportId = "AA0000000"};
    int customer_id = cudao->insert_customer(customer);
    ASSERT_GT(customer_id, 0);

    // insert company into Companies' table
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    int company_id = codao->insert_company(company);
    ASSERT_GT(company_id, 0);

    // insert flight in Flights table
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
    ASSERT_GT(flight_id, 0);

    // insert booking
    Booking booking{
        .reservationId = get_timestamp_millisecond(),
        .date = "2026-10-16",
        .isCanceled = false,
        .seatNumber = 10 // NOLINT
    };
    int booking_id = bdao->insert_booking(booking, customer_id, flight_id);
    ASSERT_GT(booking_id, 0);
    // cancel reservation
    EXPECT_GT(bdao->cancel_booking(booking.reservationId), 0);
    // verify
    auto result = bdao->get_booking(booking.reservationId);
    EXPECT_TRUE(result->isCanceled);
}

TEST_F(BookingDataAccessTest, list_bookings)
{
    // insert a customer in customers table
    Customer    customer{
           .uuid       = uuid::generate(),
           .firstName  = "Bob",
           .name       = "Kitani",
           .email      = "bob@test.com",
           .phone      = "06 22 56 32 58",
           .address    = "1 rue bobi 35000 Jamaiq",
           .gender     = "",
           .passportId = "AA0000000"};
    int customer_id = cudao->insert_customer(customer);
    ASSERT_GT(customer_id, 0);

    // insert company into Companies' table
    Company company{
        .name     = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"};
    int company_id = codao->insert_company(company);
    ASSERT_GT(company_id, 0);

    // insert flight in Flights table
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
    ASSERT_GT(flight_id, 0);

    // insert first booking
    Booking booking{
        .reservationId = get_timestamp_millisecond(),
        .date = "2026-10-16",
        .isCanceled = false,
        .seatNumber = 10 // NOLINT
    };
    ASSERT_GT(bdao->insert_booking(booking, customer_id, flight_id), 0);

    // insert second booking
    Booking booking_second{
        .reservationId = get_timestamp_millisecond() + 4, // ensure the gap
        .date = "2026-10-17",
        .isCanceled = false,
        .seatNumber = 15 // NOLINT
    };
    ASSERT_GT(bdao->insert_booking(booking_second, customer_id, flight_id), 0);

    auto bookings = bdao->list_bookings();
    EXPECT_EQ(bookings.size(), 2U);
    // check
    EXPECT_EQ(bookings[0].date, "2026-10-16");
    EXPECT_EQ(bookings[0].seatNumber, 10);

    EXPECT_EQ(bookings[1].date, "2026-10-17");
    EXPECT_EQ(bookings[1].seatNumber, 15);
}
} // namespace fisoa