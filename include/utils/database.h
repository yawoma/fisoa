#ifndef DATABASE_H
#define DATABASE_H

#include <string>

namespace fisoa {
bool        create_database_directory();
std::string get_database_path();

namespace uuid
{
std::string generate();
}

#endif    // DATABASE_H
}