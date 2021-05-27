/**
 * @file config.hpp
 * @brief Declarations for global configuration access
 * @note For now this is read only
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#if !defined(__CONFIG_HPP__)
#define __CONFIG_HPP__

#include <string>

namespace Kulike {
namespace config {

/**
 * @brief loads config file into memory
 *
 * @note file must be in json
 *
 * @param file file path relative to executable
 *
 * @return true if file was loaded correctly
 */
bool load(const std::string file);

/**
 * @brief gets a string value from the config
 *
 * @param key Key to be found, nested keys are separated by /
 *
 * @return config value or empty string if key is not found
 */
std::string get_string(const std::string key);

/**
 * @brief resets the config, must read again with @ref load
 */
void reset();

} // namespace config
} // namespace Kulike

#endif // __CONFIG_HPP__
