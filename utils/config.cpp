#include <fstream>

#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "config.hpp"

namespace Kulike {
namespace config {

static bool loaded = false;
static nlohmann::json j_config;

void load(std::string file) {
    if (loaded) {
        spdlog::warn("Trying to load config again");
        return;
    }

    try {
        std::ifstream fstream(file);
        fstream >> j_config;
    } catch (std::exception& e) {
        spdlog::error(fmt::format("Failed to open config file: {}", e.what()));
        return;
    }

    loaded = true;
}

std::string get_string(std::string key) {
    if (!loaded) {
        spdlog::warn("Trying to get config before load");
        return "";
    }

    return j_config[key];
}

} // namespace config
} // namespace Kulike
