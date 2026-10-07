#ifndef DATABASE_H
#define DATABASE_H

#include "SQLiteCpp/Database.h"
#include <string>

namespace fisoa
{
bool        create_database_directory();
std::string get_database_path();
void        open_database(SQLite::Database& database);

namespace uuid
{
std::string generate();
}

#endif    // DATABASE_H
}
