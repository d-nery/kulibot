/**
 * @file config.cpp
 * @brief Definitions for global configuration access
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#include <fstream>
#include <sstream>

#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "config.hpp"

namespace Kulike {
namespace config {

static bool loaded = false;
static nlohmann::json j_config;

bool load(const std::string file) {
    if (loaded) {
        spdlog::warn("[config] Trying to load again");
        return false;
    }

    try {
        std::ifstream fstream(file);
        fstream >> j_config;
    } catch (std::exception& e) {
        spdlog::error(fmt::format("[config] Failed to open/parse file: {}", e.what()));
        return false;
    }

    loaded = true;
    return true;
}

std::string get_string(const std::string key) {
    if (!loaded) {
        spdlog::warn("[config] Trying to get before load");
        return "";
    }

    std::istringstream ss{key};
    std::string field;
    nlohmann::json j = j_config;

    // Split key on / to find nested fields
    try {
        while (std::getline(ss, field, '/')) {
            j = j.at(field);
        }
    } catch (nlohmann::detail::out_of_range& oor) {
        spdlog::error(fmt::format("[config] Error getting value: {}", oor.what()));
        return {};
    }

    return j;
}

} // namespace config
} // namespace Kulike
