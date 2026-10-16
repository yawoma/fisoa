#include "model/airplane_data_access.h"

#include "SQLiteCpp/Database.h"
#include "utils/database.h"
#include <gtest/gtest.h>
#include <memory>
#include <optional>

namespace fisoa
{
class AirplaneDataAccessTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create SQLite DataBase in memory
        db = std::make_unique<SQLite::Database>(
            ":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        adao = std::make_unique<AirplaneDataAccess>(*db);
        ASSERT_GE(adao->create_table(), 0);
    }

    void TearDown() override
    {
        adao.reset();
        db.reset();
    }

    // NOLINTBEGIN (misc-non-private-member-variables-in-classes)
    std::unique_ptr<SQLite::Database>   db;
    std::unique_ptr<AirplaneDataAccess> adao;
    // NOLINTEND
};

TEST_F(AirplaneDataAccessTest, insert_and_get_airplane)
{
    Airplane airplane{
        .uuid            = uuid::generate(),
        .model           = "Coso C350",
        .numberRows      = 35,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .numberColumns   = 6,     // NOLINT
        .numberLanes     = 1,
        .acquisitionDate = "2026-10-07"};
    int airplane_id = adao->insert_airplane(airplane);
    EXPECT_GT(airplane_id, 0);

    auto result = adao->get_airplane(airplane.uuid);

    EXPECT_EQ(result->id, airplane_id);
    EXPECT_EQ(result->uuid, airplane.uuid);
    EXPECT_EQ(result->model, "Coso C350");
    EXPECT_EQ(result->numberRows, 35);
    EXPECT_EQ(result->numberColumns, 6);
    EXPECT_EQ(result->numberLanes, 1);
    EXPECT_EQ(result->acquisitionDate, "2026-10-07");
}

TEST_F(AirplaneDataAccessTest, get_non_existing_airplane)
{
    auto result = adao->get_airplane("does-not-exist-uuid");
    EXPECT_EQ(result, std::nullopt);
}

TEST_F(AirplaneDataAccessTest, unique_uuid)
{
    auto     uuid = uuid::generate();
    Airplane airplane{
        .uuid            = uuid,
        .model           = "Coso C350",
        .numberRows      = 35,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .numberColumns   = 6,     // NOLINT
        .numberLanes     = 1,
        .acquisitionDate = "2026-10-07"};
    int      airplane_id = adao->insert_airplane(airplane);

    // diff aircraft with same uuid
    Airplane airplane2{
        .uuid            = uuid,
        .model           = "Casa CA350",
        .numberRows      = 45,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .numberColumns   = 6,     // NOLINT
        .numberLanes     = 1,
        .acquisitionDate = "2025-10-07"};
    // Faile to insert: result will be nill
    EXPECT_EQ(adao->insert_airplane(airplane2), 0);
}

TEST_F(AirplaneDataAccessTest, update_airplane)
{
    Airplane airplane{
        .uuid            = uuid::generate(),
        .model           = "Coso C350",
        .numberRows      = 35,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .numberColumns   = 6,     // NOLINT
        .numberLanes     = 1,
        .acquisitionDate = "2026-10-07"};
    int  airplane_id        = adao->insert_airplane(airplane);
    auto result             = adao->get_airplane(airplane.uuid);

    // update rows, cols, and date
    result->numberRows      = 40;    // NOLINT
    result->numberColumns   = 8;     // NOLINT
    result->acquisitionDate = "2025-07-20";
    EXPECT_GT(adao->update_airplane(*result), 0);
    // check
    auto result2 = adao->get_airplane(result->uuid);
    EXPECT_EQ(result2->numberRows, 40);
    EXPECT_EQ(result2->numberColumns, 8);
    EXPECT_EQ(result2->acquisitionDate, "2025-07-20");
}

TEST_F(AirplaneDataAccessTest, update_non_existing_airplane)
{
    Airplane airplane{
        .uuid            = "does-not-exist-airplane",
        .model           = "Coso C350",
        .numberRows      = 35,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .numberColumns   = 6,     // NOLINT
        .numberLanes     = 1,
        .acquisitionDate = "2026-10-07"};
    EXPECT_EQ(adao->update_airplane(airplane), 0);
}

TEST_F(AirplaneDataAccessTest, delete_airplane)
{
    Airplane airplane{
        .uuid            = uuid::generate(),
        .model           = "Coso C350",
        .numberRows      = 35,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .numberColumns   = 6,     // NOLINT
        .numberLanes     = 1,
        .acquisitionDate = "2026-10-07"};
    int airplane_id = adao->insert_airplane(airplane);

    EXPECT_GT(adao->delete_aiplane(airplane.uuid), 0);
    // check
    EXPECT_EQ(adao->get_airplane(airplane.uuid), std::nullopt);

    /*deleting non existing airplane*/
    EXPECT_EQ(adao->delete_aiplane("does-not-exist-uuid"), 0);
}

TEST_F(AirplaneDataAccessTest, list_airplanes)
{
    Airplane airplane{
        .uuid            = uuid::generate(),
        .model           = "Coso C350",
        .numberRows      = 35,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .numberColumns   = 6,     // NOLINT
        .numberLanes     = 1,
        .acquisitionDate = "2026-10-07"};
    ASSERT_GT(adao->insert_airplane(airplane), 0);

    Airplane airplane2{
        .uuid            = uuid::generate(),
        .model           = "Casa CA500",
        .numberRows      = 45,    // NOLINT (cppcoreguidelines-avoid-magic-numbers)
        .numberColumns   = 10,    // NOLINT
        .numberLanes     = 2,
        .acquisitionDate = "2026-07-10"};
    ASSERT_GT(adao->insert_airplane(airplane2), 0);

    auto airplanes = adao->list_airplanes();
    ASSERT_EQ(airplanes.size(), 2U);
    // check
    EXPECT_EQ(airplanes[0].model, "Coso C350");
    EXPECT_EQ(airplanes[1].model, "Casa CA500");
}

}    // namespace fisoa
