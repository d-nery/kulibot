#if !defined(__CHARACTERS_HPP__)
#define __CHARACTERS_HPP__

#include <string>
#include <vector>

namespace Kulike {
namespace characters {

struct Character {
    std::string name;
    std::string race;
    std::vector<std::string> classes;
    std::string image;
    std::string background;
    std::string alignment;
    std::unordered_map<std::string, int> stats;
    std::vector<std::string> proeficiencies;
    int max_hp;
    int ac;
};

void load(const std::string folder);

std::vector<std::string> list();
Character get(const std::string key);

} // namespace characters
} // namespace Kulike

#endif // __CHARACTERS_HPP__
