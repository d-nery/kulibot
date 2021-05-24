/**
 * @file inventory.hpp
 * @brief Declarations for global inventory access
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#if !defined(__INVENTORY_HPP__)
#define __INVENTORY_HPP__

#include <string>
#include <vector>

namespace Kulike {
namespace inventory {

/**
 * @brief Represents one item in the inventory
 */
struct Item {
    std::string name;        //!< Item name
    std::string description; //!< Item description
    int amount;              //!< Item amount
};

/**
 * @brief loads inventory file into memory
 *
 * @note file must be in json
 *
 * @param file file path relative to executable
 *
 * @return true if file was loaded correctly
 */
bool load(const std::string file);

/**
 * @brief saves inventory from memory into file, in json
 *
 * @param file file path relative to executable
 */
void save(const std::string file);

/**
 * @brief resets the inventory, must read again with @ref load
 */
void reset();

/**
 * @brief gets the amount of gold in the inventory
 *
 * @return the amount of gold
 */
uint64_t get_gold();

/**
 * @brief gets the amount of gold in the inventory
 *
 * @return list of categories
 */
std::vector<std::string> get_categories();

/**
 * @brief gets all the items from the supplied category
 *
 * @param category category to fetch
 *
 * @return list of items
 */
std::vector<Item> get_items(const std::string category);

/**
 * @brief add an item to the inventory
 *
 * @note ::save is not called automatically, user must save
 *       the inventory when appropriate
 *
 * @param category category to add to
 * @param item Item to be added
 */
void add_item(const std::string category, const Item item);

/**
 * @brief add or remove gold from the inventory
 *
 * @note ::save is not called automatically, user must save
 *       the inventory when appropriate
 *
 * @param category value to add (negative values remove)
 */
void add_gold(const int value);

} // namespace inventory
} // namespace Kulike

#endif // __INVENTORY_HPP__
