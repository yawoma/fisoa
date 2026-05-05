#ifndef DATABASE_H
#define DATABASE_H

#include <string>

bool create_database_directory();

namespace uuid {
    std::string generate();
}

#endif // DATABASE_H