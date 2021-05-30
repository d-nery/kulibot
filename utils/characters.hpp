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

const std::vector<std::string> stats{"str", "dex", "con", "int", "wis", "cha"};
const std::unordered_map<std::string, std::string> skills{
    {"acrobatics", "dex"},    {"animal handling", "wis"}, {"arcana", "int"},   {"athletics", "str"},
    {"deception", "cha"},     {"history", "int"},         {"insight", "wis"},  {"intimidation", "cha"},
    {"investigation", "int"}, {"medicine", "wis"},        {"nature", "int"},   {"perception", "wis"},
    {"performance", "cha"},   {"persuasion", "cha"},      {"religion", "int"}, {"sleight of hand", "dex"},
    {"stealth", "dex"},       {"survival", "wis"},
};

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
    std::vector<std::string> proficiencies;     //!< List of character trained proficiencies
    int max_hp;                                 //!< Character maximum health
    int ac;                                     //!< Character armor class
    int prof;                                   //!< Character proficiency bonus
    uint64_t owner;                             //!< Id of the character owner

    /**
     * @brief rolls a d20
     */
    uint32_t roll();

    /**
     * @brief gets the modifier for the given stat
     */
    int modifier(std::string stat);
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
 * @brief resets the characters, must read again with @ref load
 */
void reset();

/**
 * @brief gets a character from memory
 *
 * @param key Character key
 */
Character get(const std::string key);

/**
 * @brief gets a character by ownser id
 *
 * @param owner owner id
 */
Character get(const uint64_t owner);

} // namespace characters
} // namespace Kulike

#endif // __CHARACTERS_HPP__
