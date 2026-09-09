#include "utils/database.h"

#include "utils/logger.h"
#include "uuid.h"

#include <algorithm>
#include <filesystem>
#include <string>

namespace fisoa{
bool create_database_directory()
{
    std::string           path = std::string(SOURCE_DIR) + "/data/fisoa.db3";
    std::filesystem::path dir = std::filesystem::path(path).parent_path();

    if (!std::filesystem::exists(dir))
    {
        try
        {
            std::filesystem::create_directories(dir);
            return true;
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::string err_msg =
                "Error creating database directory: " + std::string(e.what());
            get_logger().error(err_msg);
            return false;
        }
    }
    return true;    // Directory already exists
}

std::string get_database_path() { return std::string(SOURCE_DIR) + "/data/fisoa.db3"; }

namespace uuid
{
std::string generate() 
{ 
    std::random_device rdevice;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::ranges::generate(seed_data.begin(), seed_data.end(), std::ref(rdevice));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{generator};

    return uuids::to_string(gen()); 
}
}    // namespace uuid
}    // namespace fisoa
