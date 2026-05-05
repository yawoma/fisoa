#include "utils/database.h"

#include "utils/logger.h"

#include <filesystem>
#include <string>

#define UUID_SYSTEM_GENERATOR
#include "uuid.h"


bool create_database_directory()
{
    std::string path = std::string(SOURCE_DIR) + "/data/fisoa.db3";
    std::filesystem::path db_path(path);
    std::filesystem::path dir = db_path.parent_path();

    if (!std::filesystem::exists(dir))
    {
        try
        {
            std::filesystem::create_directories(dir);
            return true;
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::string err_msg = "Error creating database directory: "  + std::string(e.what());
            get_logger().error(err_msg);
            return false;
        }
    }
    return true; // Directory already exists
}

namespace uuid {
    std::string generate() {
        return uuids::to_string(uuids::uuid_system_generator{}());
    }
}