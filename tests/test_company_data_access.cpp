#include "model/company_data_access.h"

#include <SQLiteCpp/Database.h>
#include <gtest/gtest.h>
#include <memory>

namespace fisoa {
class CompanyDataAccessTest : public ::testing::Test {
    protected:
        void SetUp() override
        {
            // Create SQLite DataBase in memory
            db = std::make_unique<SQLite::Database>(":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            // Execute with the corresponding schema
            int result = db->exec(
                "CREATE TABLE Companies ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT NOT NULL,"
                "codeIcao TEXT NOT NULL UNIQUE,"
                "codeIata TEXT,"
                "callSign TEXT"
                ")"
            );
            ASSERT_EQ(result, 0);
            ASSERT_TRUE(db->tableExists("Companies"));

            cdao = std::make_unique<CompanyDataAccess>(*db);
        }

        void TearDown() override {
            cdao.reset();
            db.reset();
        }

        // NOLINTBEGIN (misc-non-private-member-variables-in-classes)
        std::unique_ptr<SQLite::Database> db;
        std::unique_ptr<CompanyDataAccess> cdao;
        // NOLINTEND
};

TEST_F(CompanyDataAccessTest, insert_and_get_company)
{
    Company company{
        .name = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"
    };
    int company_id = cdao->insert_company(company);
    EXPECT_GT(company_id, 0);

    auto result = cdao->get_company("ABB");

    EXPECT_EQ(result.id, company_id);
    EXPECT_EQ(result.name, "Air Bambi");
    EXPECT_EQ(result.codeIcao, "ABB");
    EXPECT_EQ(result.codeIata, "AB");
    EXPECT_EQ(result.callSign, "Smallbeast");
}

TEST_F(CompanyDataAccessTest, get_non_exist_company)
{
    auto result = cdao->get_company("AFR");
    EXPECT_EQ(result.id, 0); // id will be the default value 0
}

TEST_F(CompanyDataAccessTest, unique_icao)
{
    Company company{
        .name = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"
    };
    int company_id = cdao->insert_company(company);

    // same icao o another company
    Company company2{
        .name = "Air Panpan",
        .codeIcao = "ABB",
        .codeIata = "AP",
        .callSign = "Verysmallbeast"
    };
    // it will faile to insert then the result will be nill
    EXPECT_EQ(cdao->insert_company(company2), 0);
}

TEST_F(CompanyDataAccessTest, update_company)
{
    Company company{
        .name = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"
    };
    int company_id = cdao->insert_company(company);
    auto result = cdao->get_company("ABB");

    // update iata and call sign
    result.codeIata = "AI";
    result.callSign = "Littlebeast";
    EXPECT_GT(cdao->update_company(result), 0);
    // verify
    result = cdao->get_company("ABB");
    EXPECT_EQ(result.codeIata, "AI"); // old: AB
    EXPECT_EQ(result.callSign, "Littlebeast"); // old: Littlebeast
}

TEST_F(CompanyDataAccessTest, update_non_exist_company)
{
    Company company{
        .name = "Does not exist",
        .codeIcao = "DNE",
        .codeIata = "BN",
        .callSign = ""
    };
    EXPECT_EQ(cdao->update_company(company), 0);
}

TEST_F(CompanyDataAccessTest, delete_company)
{
    Company company{
        .name = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"
    };
    int company_id = cdao->insert_company(company);

    // delete the last inserted
    EXPECT_GT(cdao->delete_company(company.codeIcao), 0);
    // verify
    EXPECT_EQ(cdao->get_company(company.codeIcao).id, 0);
}

TEST_F(CompanyDataAccessTest, delete_non_exist_company)
{
    EXPECT_EQ(cdao->delete_company("does not exist"), 0);
}

TEST_F(CompanyDataAccessTest, list_companies)
{
    // first company
    Company company{
        .name = "Air Bambi",
        .codeIcao = "ABB",
        .codeIata = "AB",
        .callSign = "Smallbeast"
    };
    EXPECT_GT(cdao->insert_company(company), 0);

    // second
    Company company2{
        .name = "Air Panpan",
        .codeIcao = "APP",
        .codeIata = "AP",
        .callSign = "Verysmallbeast"
    };
    EXPECT_GT(cdao->insert_company(company2), 0);

    auto companies = cdao->list_companies();
    ASSERT_EQ(companies.size(), 2U);
    // verify
    EXPECT_EQ(companies[0].name, "Air Bambi");
    EXPECT_EQ(companies[1].name, "Air Panpan");
}
}  // namespace fisoa