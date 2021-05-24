#include <filesystem>
#include <fstream>
#include <sstream>

#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "characters.hpp"

namespace Kulike {
namespace characters {

namespace fs = std::filesystem;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Character, name, race, classes, background, alignment, image, stats, proeficiencies,
                                   max_hp, ac);

static bool loaded = false;
static std::unordered_map<std::string, Character> char_map;

void load(const std::string folder) {
    if (loaded) {
        spdlog::warn("[characters] Trying to load characters again");
        return;
    }

    fs::path path = folder;

    for (auto& file : fs::directory_iterator(path)) {
        if (!file.is_regular_file() || file.path().extension() != ".json") {
            continue;
        }

        std::string key = file.path().stem();
        spdlog::debug(fmt::format("[character] Loading {}", key));

        nlohmann::json char_data;
        Character character;

        try {
            std::ifstream fstream(file.path());
            fstream >> char_data;
            char_data.get_to(character);
        } catch (std::exception& e) {
            spdlog::error(fmt::format("Failed to open/parse char file: {}", e.what()));
            continue;
        }

        char_map[key] = character;
    }

    spdlog::debug(fmt::format("[characters] Loaded {} character(s) to map", char_map.size()));

    loaded = true;
}

std::vector<std::string> list() {
    std::vector<std::string> keys;
    keys.reserve(char_map.size());

    for (auto& [k, _] : char_map) {
        keys.push_back(k);
    }

    return keys;
}

Character get(const std::string key) {
    if (char_map.contains(key)) {
        return char_map[key];
    }

    return {};
}

} // namespace characters
} // namespace Kulike
