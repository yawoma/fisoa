#ifndef DATABASE_H
#define DATABASE_H

#include "SQLiteCpp/Database.h"
#include <cstdint>
#include <optional>
#include <string>

namespace fisoa
{
bool        create_database_directory();
std::string get_database_path();
void        open_database(SQLite::Database& database);

class DatabaseManager
{
public:
    static std::optional<SQLite::Database> instance();
};

/**Getting millisecond timestamp since the Unix epoch*/
uint64_t get_timestamp_millisecond();

namespace uuid
{
std::string generate();
}

#endif    // DATABASE_H
}
