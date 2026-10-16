#ifndef BOOKING_DATA_ACCESS_H
#define BOOKING_DATA_ACCESS_H

#include "SQLiteCpp/Database.h"
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

constexpr int64_t ONE_YEAR = 1823644800; ///< one year in timestamp

namespace fisoa {
struct Booking{
    uint32_t id = 0;
    std::string customerUUID;
    std::string flightUUID;
    int64_t reservationId; ///< reservation date as timestamp
    std::string date;
    bool isCanceled;
    uint32_t seatNumber = 0;
};

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class BookingDataAccess{
    public:
        explicit BookingDataAccess(SQLite::Database& database);
        ~BookingDataAccess() = default;

        BookingDataAccess(const BookingDataAccess&) = delete;
        /*move assignement implicitly deleted due to reference database m_db*/
        BookingDataAccess& operator=(const BookingDataAccess&) = delete;

        int insert_booking(const Booking& booking, uint32_t customer_id, uint32_t flight_id);
        int update_booking(const Booking& booking);
        int delete_booking(int64_t reservation_id);
        int cancel_booking(int64_t reservation_id);
        std::optional<Booking> get_booking(int64_t reservation_id);
        std::vector<Booking> list_bookings();

        int create_table();
    private:
        SQLite::Database& m_db;
};
} // namespace fisoa
#endif // BOOKING_DATA_ACCESS_H