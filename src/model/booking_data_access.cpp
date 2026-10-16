#include "model/booking_data_access.h"

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Exception.h"
#include "SQLiteCpp/Statement.h"
#include "utils/database.h"
#include "utils/logger.h"
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace fisoa
{
BookingDataAccess::BookingDataAccess(SQLite::Database& database): m_db(database) {}

int BookingDataAccess::insert_booking(
    const Booking& booking, uint32_t customer_id, uint32_t flight_id)
{
    SQLite::Statement query(
        m_db,
        "INSERT INTO Bookings "
        "(customerUuid, flightUuid, reservationId, reservationDate, reservationCanceled, "
        "seatNumber) VALUES ((SELECT customer.uuid FROM Customers customer WHERE "
        "customer.id = ?), "
        "(SELECT flight.uuid FROM Flights flight WHERE flight.id = ?), ?, ?, ?, ?)");
    const int CUSTOMER_UUID_VAL = 1;
    query.bind(CUSTOMER_UUID_VAL, customer_id);
    const int FLIGHT_UUID_VAL = 2;
    query.bind(FLIGHT_UUID_VAL, flight_id);
    const int RESERVATION_ID_VAL = 3;
    query.bind(RESERVATION_ID_VAL, booking.reservationId);
    const int RESERVATION_DATE_VAL = 4;
    query.bind(RESERVATION_DATE_VAL, booking.date);
    const int CANCEL_VAL = 5;
    query.bind(CANCEL_VAL, static_cast<int32_t>(booking.isCanceled));
    const int SEAT_VAL = 6;
    query.bind(SEAT_VAL, booking.seatNumber);
    int result = 0;
    try
    {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info(
                "Booking " + std::to_string(booking.reservationId) +
                " has been booked successfully");
        }
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error(
            "Failed to book booking " + std::to_string(booking.reservationId) + " : " +
            e.what());
    }
    return result;
}

int BookingDataAccess::update_booking(const Booking& booking)
{
    SQLite::Statement query(
        m_db, "UPDATE Bookings SET reservationId = ?, "
              "reservationDate = ?, seatNumber = ? WHERE customerUuid = ?");
    query.bind(1, booking.reservationId);
    query.bind(2, booking.date);
    query.bind(3, booking.seatNumber);
    query.bind(4, booking.customerUUID);
    int result = 0;
    try
    {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info(
                "Customer uuid " + booking.customerUUID +
                " booking has been updated successfully");
        }
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error(
            "Failed to updated customer uuid " + booking.customerUUID +
            " booking : " + e.what());
    }
    return result;
}

int BookingDataAccess::delete_booking(int64_t reservation_id)
{
    // reservation must be able to be deleted only after one year
    const int64_t     diff_time = get_timestamp_millisecond() - ONE_YEAR;
    SQLite::Statement query(
        m_db, "DELETE FROM Bookings WHERE reservationId = ? "
              "AND reservationId <= ?");
    query.bind(1, reservation_id);
    query.bind(2, diff_time);
    int result = 0;
    try
    {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info(
                "Booking " + std::to_string(reservation_id) +
                " has been deleted successfully");
        }
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error(
            "Failed to delete booking " + std::to_string(reservation_id) + " : " +
            e.what());
    }
    return result;
}

int BookingDataAccess::cancel_booking(int64_t reservation_id)
{
    SQLite::Statement query(
        m_db, "UPDATE Bookings SET reservationCanceled = ? WHERE reservationId = ?");
    const int TRUE = 1;
    query.bind(1, TRUE);
    query.bind(2, reservation_id);
    int result = 0;
    try
    {
        result = query.exec();
        if (result > 0)
        {
            get_logger().info(
                "Booking " + std::to_string(reservation_id) + " has been cancled");
        }
    }
    catch (const SQLite::Exception& e)
    {
        get_logger().error(
            "Failed to cancel booking " + std::to_string(reservation_id) + " : " +
            e.what());
    }
    return result;
}

std::optional<Booking> BookingDataAccess::get_booking(int64_t reservation_id)
{
    SQLite::Statement query(
        m_db, "SELECT id, customerUuid, flightUuid, reservationId, reservationDate, "
              "reservationCanceled, seatNumber FROM Bookings WHERE reservationId = ?");
    query.bind(1, reservation_id);
    if (query.executeStep())
    {
        return Booking{
            .id            = query.getColumn("id").getUInt(),
            .customerUUID  = query.getColumn("customerUuid").getString(),
            .flightUUID    = query.getColumn("flightUuid").getString(),
            .reservationId = query.getColumn("reservationId").getInt64(),
            .date          = query.getColumn("reservationDate").getString(),
            .isCanceled =
                static_cast<bool>(query.getColumn("reservationCanceled").getInt()),
            .seatNumber = query.getColumn("seatNumber").getUInt()};
    }
    return std::nullopt;
}

std::vector<Booking> BookingDataAccess::list_bookings()
{
    SQLite::Statement query(
        m_db, "SELECT id, customerUuid, flightUuid, reservationId, reservationDate, "
              "reservationCanceled, seatNumber FROM Bookings");
    std::vector<Booking> bookings;
    while (query.executeStep())
    {
        bookings.push_back(
            Booking{
                .id            = query.getColumn("id").getUInt(),
                .customerUUID  = query.getColumn("customerUuid").getString(),
                .flightUUID    = query.getColumn("flightUuid").getString(),
                .reservationId = query.getColumn("reservationId").getInt64(),
                .date          = query.getColumn("reservationDate").getString(),
                .isCanceled =
                    static_cast<bool>(query.getColumn("reservationCanceled").getInt()),
                .seatNumber = query.getColumn("seatNumber").getUInt()});
    }
    return bookings;
}

int BookingDataAccess::create_table()
{
    int result = -1;
    if (m_db.getHandle() != nullptr)
    {
        // if customer table and flight table does not exist then skip
        if (!m_db.tableExists("Customers") && !m_db.tableExists("Flights"))
        {
            return result;
        }
        // create now Bookings table if not exist
        if (!m_db.tableExists("Bookings"))
        {
            result = m_db.exec(
                "CREATE TABLE Bookings ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "customerUuid TEXT NOT NULL, "
                "flightUuid TEXT NOT NULL, "
                "reservationId INTEGER UNIQUE, "
                "reservationDate TEXT NOT NULL, "
                "reservationCanceled BOOLEAN, "
                "seatNumber INTEGER, "
                "FOREIGN KEY (customerUuid) REFERENCES Customers(uuid)"
                "   ON DELETE RESTRICT, "
                "FOREIGN KEY (flightUuid) REFERENCES Flights(uuid)"
                "   ON DELETE RESTRICT"
                ")");
            if (result >= 0)
            {
                get_logger().info("Bookings table created successfully");
            }
        }
        else
        {
            get_logger().info("Bookings table found");
            result = 0;
        }
        return result;
    }
    get_logger().error("Database has not been connceted");
    return result;
}
}    // namespace fisoa
