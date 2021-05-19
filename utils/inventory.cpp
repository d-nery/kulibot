#include <fstream>

#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "inventory.hpp"

namespace Kulike {
namespace inventory {

static struct s_inv {
    uint64_t gold;

    /* "category1": [
            ["item1", 0],
            ["item2", 1],
            ["item3", 2],
            ["item4", 3],
            ["item5", 4]
        ],
        "category2": ...
     */
    std::map<std::string, std::vector<std::map<std::string, int>>> items;
} inventory;

static bool loaded = false;

void from_json(nlohmann::json& j, s_inv& inv) {
    j.at("gold").get_to(inv.gold);

    j.at("items");
}

void load(std::string file) {
    if (loaded) {
        spdlog::warn("Trying to load inventory again");
        return;
    }

    try {
        nlohmann::json j;
        std::ifstream fstream(file);
        fstream >> j;
        // inventory = j.get<s_inv>();
    } catch (std::exception& e) {
        spdlog::error(fmt::format("Failed to open inventory file: {}", e.what()));
        return;
    }

    loaded = true;
}

uint64_t get_gold() {
    if (!loaded) {
        spdlog::warn(fmt::format("Tried to read inventory before loading"));
        return 0;
    }

    return inventory.gold;
}

} // namespace inventory
} // namespace Kulike
