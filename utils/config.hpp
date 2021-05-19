#if !defined(__CONFIG_HPP__)
#define __CONFIG_HPP__

#include <string>

namespace Kulike {
namespace config {

void load(std::string file);

std::string get_string(std::string key);

} // namespace config
} // namespace Kulike

#endif // __CONFIG_HPP__
