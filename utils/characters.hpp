/**
 * @file characters.hpp
 * @brief Declarations for global characters access
 * @note For now this is read only
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#if !defined(__CHARACTERS_HPP__)
#define __CHARACTERS_HPP__

#include <string>
#include <vector>

namespace Kulike {
namespace characters {

/**
 * @brief This represents a DnD character sheet
 */
struct Character {
    std::string name;                           //!< Character name
    std::string race;                           //!< Character race
    std::vector<std::string> classes;           //!< List of character classes
    std::string image;                          //!< Character profile image
    std::string background;                     //!< Character background
    std::string alignment;                      //!< Character alignment
    std::unordered_map<std::string, int> stats; //!< Map of charater stats and values
    std::vector<std::string> proeficiencies;    //!< List of character trained proeficiencies
    int max_hp;                                 //!< Character maximum health
    int ac;                                     //!< Character armor class
};

/**
 * @brief loads all characters into memory
 * This reads all .json files on the given folder and parse them
 * into characters, their key will be the file stem
 *
 * @param folder path relative to executable
 *
 * @return true if file was loaded correctly
 */
bool load(const std::string folder);

/**
 * @brief list all loaded characters keys
 *
 * @return vector of characters key
 */
std::vector<std::string> list();

/**
 * @brief gets a character from memory
 *
 * @param key Character key
 */
Character get(const std::string key);

} // namespace characters
} // namespace Kulike

#endif // __CHARACTERS_HPP__
