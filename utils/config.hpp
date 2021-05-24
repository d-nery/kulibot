#if !defined(__CONFIG_HPP__)
#define __CONFIG_HPP__

#include <string>

namespace Kulike {
namespace config {

void load(const std::string file);

/**
 * @brief gets a string value from the config
 *
 * @param key Key to be found, nested keys are separated by /
 */
std::string get_string(const std::string key);

} // namespace config
} // namespace Kulike

#endif // __CONFIG_HPP__
