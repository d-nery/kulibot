#if !defined(__INVENTORY_HPP__)
#define __INVENTORY_HPP__

#include <string>
#include <vector>

namespace Kulike {
namespace inventory {

struct Item {
    std::string name;
    std::string description;
    int amount;
};

void load(const std::string file);
void save(const std::string file);
void reset();

uint64_t get_gold();
std::vector<std::string> get_categories();
std::vector<Item> get_items(const std::string category);

void add_item(const std::string category, const Item item);
void add_gold(const int value); // add or remove with negative number

} // namespace inventory
} // namespace Kulike

#endif // __INVENTORY_HPP__
