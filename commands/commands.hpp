/**
 * @file command.hpp
 * @brief All the command declarations,
 *        each one is defined in their own .cpp file
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#if !defined(__COMMANDS_HPP__)
#define __COMMANDS_HPP__

#include "bot/Command.hpp"

/**
 * @brief Helper macro for command declaration
 */
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
cmd_decl(RollCmd);

} // namespace Kulike

#endif // __COMMANDS_HPP__
