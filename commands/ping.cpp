/**
 * @file ping.cpp
 * @brief This commands justa answers the message,
 *        can be used to check if bot is alive
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#include <dpp/dpp.h>
#include <spdlog/spdlog.h>

#include "commands.hpp"

namespace Kulike {

PingCmd::PingCmd() {
    this->key = "ping";
    this->_command.set_name(this->key).set_description("Pong!");
}

bool PingCmd::run(const dpp::interaction_create_t& event) {
    event.reply(dpp::ir_channel_message_with_source, "Pong!");
    return true;
}

PingCmd::~PingCmd() {}

} // namespace Kulike
