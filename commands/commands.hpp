#if !defined(__COMMANDS_HPP__)
#define __COMMANDS_HPP__

#include "bot/Command.hpp"

namespace Kulike {

struct PingCmd : Command {
    PingCmd();
    bool run(const dpp::interaction_create_t& event) override;
};

json read_boh(void);

struct BOHCmd : Command {
    BOHCmd();
    bool run(const dpp::interaction_create_t& event) override;
};

} // namespace Kulike

#endif // __COMMANDS_HPP__
