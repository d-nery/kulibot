#if !defined(__COMMANDS_HPP__)
#define __COMMANDS_HPP__

#include "bot/Command.hpp"

#define cmd_decl(name)                                                                                                 \
    struct name : Command {                                                                                            \
        name();                                                                                                        \
        ~name();                                                                                                       \
        bool run(const dpp::interaction_create_t& event) override;                                                     \
    }

namespace Kulike {

cmd_decl(PingCmd);
cmd_decl(BOHCmd);
cmd_decl(CharCmd);

} // namespace Kulike

#endif // __COMMANDS_HPP__
