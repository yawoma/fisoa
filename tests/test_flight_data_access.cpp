#include "model/flight_data_access.h"

#include "model/company_data_access.h"
#include "SQLiteCpp/Database.h"

#include <gtest/gtest.h>
#include <memory>

namespace fisoa {
class FlightDataAccessTest : public ::testing::Test{
    protected:
        void SetUp() override {
            // Create SQLite DataBase in memory
            db = std::make_unique<SQLite::Database>(
            ":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            cdao = std::make_unique<CompanyDataAccess>(*db);
        }

        // NOLINTBEGIN (misc-non-private-member-variables-in-classes)
        std::unique_ptr<SQLite::Database> db;
        std::unique_ptr<CompanyDataAccess> cdao;
        std::unique_ptr<FlightDataAccess> fdao;
        // NOLINTEND
};
} // namespace fisoa