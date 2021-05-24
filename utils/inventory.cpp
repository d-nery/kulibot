#include <fstream>
#include <iostream>
#include <mutex>

#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "inventory.hpp"

namespace Kulike {
namespace inventory {

struct s_inv {
    uint64_t gold;

    /* "category1": [
            {
                "name": "item1",
                "description": "",
                "amount": 0
            },
            {
                "name": "item2",
                "description": "",
                "amount": 1
            }
        ],
        "category2": ...
     */
    std::map<std::string, std::vector<Item>> items;
};

static std::mutex inv_mutex;
static s_inv inventory;
static bool loaded = false;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Item, name, description, amount)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(s_inv, gold, items)

void load(const std::string file) {
    if (loaded) {
        spdlog::warn("[inventory] Trying to load again");
        return;
    }

    spdlog::info(fmt::format("[inventory] Loading {}", file));

    try {
        std::scoped_lock _(inv_mutex);
        nlohmann::json j;
        std::ifstream fstream(file);
        fstream >> j;
        j.get_to(inventory);
    } catch (std::exception& e) {
        spdlog::error(fmt::format("[inventory] Failed to open/parse inventory file: {}", e.what()));
        return;
    }

    loaded = true;
}

void save(const std::string file) {
    std::scoped_lock _(inv_mutex);
    try {
        nlohmann::json j = inventory;
        std::ofstream fstream(file);
        fstream << j.dump(2);
    } catch (std::exception& e) {
        spdlog::error(fmt::format("[inventory] Failed to save: {}", e.what()));
    }
}

void reset() {
    std::scoped_lock _(inv_mutex);
    inventory = {};
    loaded = false;
}

uint64_t get_gold() {
    if (!loaded) {
        spdlog::warn(fmt::format("[inventory] Tried to read before loading"));
        return 0;
    }

    std::scoped_lock _(inv_mutex);
    return inventory.gold;
}

std::vector<std::string> get_categories() {
    if (!loaded) {
        spdlog::warn(fmt::format("[inventory] Tried to read before loading"));
        return {};
    }

    std::scoped_lock _(inv_mutex);
    std::vector<std::string> categories;
    for (auto& [k, v] : inventory.items) {
        categories.push_back(k);
    }

    return categories;
}

std::vector<Item> get_items(const std::string category) {
    if (!loaded) {
        spdlog::warn(fmt::format("[inventory] Tried to read before loading"));
        return {};
    }

    std::scoped_lock _(inv_mutex);
    if (!inventory.items.contains(category)) {
        spdlog::warn(fmt::format("[inventory] Tried to read unknown category"));
        return {};
    }

    return inventory.items[category];
}

void add_item(const std::string category, const Item item) {
    std::scoped_lock _(inv_mutex);
    if (!inventory.items.contains(category)) {
        spdlog::warn(fmt::format("[inventory] Tried to add to unknown category, category will be created"));
        inventory.items[category] = {};
    }

    auto& items = inventory.items[category];

    if (find_if(items.begin(), items.end(), [&item](const Item& obj) { return obj.name == item.name; }) !=
        items.end()) {
        spdlog::warn(fmt::format("[inventory] Item already exists!"));
        return;
    }

    items.push_back(item);
}

void add_gold(const int value) {
    std::scoped_lock _(inv_mutex);
    inventory.gold += value;
}

} // namespace inventory
} // namespace Kulike
