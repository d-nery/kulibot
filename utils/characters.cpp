/**
 * @file characters.cpp
 * @brief Definitions for global characters access
 * @note For now this is read only
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>

#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "characters.hpp"

namespace Kulike {
namespace characters {

namespace fs = std::filesystem;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Character, name, race, classes, background, alignment, image, stats, proficiencies,
                                   max_hp, ac, prof, owner);

static bool loaded = false;
static std::unordered_map<std::string, Character> char_map;

uint32_t Character::roll() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distribution(1, 20);
    int dice_roll = distribution(rng);

    return dice_roll;
}

int Character::modifier(std::string stat) {
    bool is_ability_check = false;
    stat = stat == "init" ? "dex" : stat;

    std::string base_stat = stat;

    if (!stats.contains(stat)) {
        if (!skills.contains(stat)) {
            return 0;
        }

        is_ability_check = true;
        base_stat = skills.at(stat);
    }

    int extra = this->prof * std::count(this->proficiencies.begin(), this->proficiencies.end(), stat);

    bool is_bard = std::find_if(this->classes.begin(), this->classes.end(), [](std::string& elem) {
                       return elem.find("Bard") != std::string::npos;
                   }) != this->classes.end();

    // If the class is bard we add half the proficiency to every ability check
    // that doesn't have a proficiency (Jack of All Trades)
    if (extra == 0 && is_ability_check && is_bard) {
        extra = this->prof / 2;
    }

    return std::floor((stats[base_stat] - 10) / 2.0) + extra;
}

bool load(const std::string folder) {
    if (loaded) {
        spdlog::warn("[characters] Trying to load characters again");
        return false;
    }

    fs::path path = folder;

    for (auto& file : fs::directory_iterator(path)) {
        if (!file.is_regular_file() || file.path().extension() != ".json") {
            continue;
        }

        std::string key = file.path().stem();
        spdlog::debug(fmt::format("[characters] Loading {}", key));

        nlohmann::json char_data;
        Character character;

        try {
            std::ifstream fstream(file.path());
            fstream >> char_data;
            char_data.get_to(character);
        } catch (std::exception& e) {
            spdlog::error(fmt::format("[characters] Failed to open/parse char file: {}", e.what()));
            continue;
        }

        char_map[key] = character;
    }

    spdlog::debug(fmt::format("[characters] Loaded {} character(s) to map", char_map.size()));

    loaded = true;
    return true;
}

std::vector<std::string> list() {
    std::vector<std::string> keys;
    keys.reserve(char_map.size());

    for (auto& [k, _] : char_map) {
        keys.push_back(k);
    }

    return keys;
}

void reset() {
    char_map = {};
    loaded = false;
}

Character get(const std::string key) {
    if (char_map.contains(key)) {
        return char_map[key];
    }

    return {};
}

Character get(const uint64_t owner) {
    for (auto& [k, c] : char_map) {
        if (c.owner == owner) {
            return c;
        }
    }

    return {};
}

} // namespace characters
} // namespace Kulike
