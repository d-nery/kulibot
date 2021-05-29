/**
 * @file reload.cpp
 * @brief This command rereads all files, only usable by bot owner
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#include <dpp/dpp.h>
#include <spdlog/spdlog.h>

#include "commands.hpp"

#include "utils/characters.hpp"
#include "utils/config.hpp"
#include "utils/inventory.hpp"

namespace Kulike {

ReloadCmd::ReloadCmd() {
    this->key = "reload";
    this->_command.set_name(this->key)
        .set_description("Reloads files")
        .disable_default_permissions()
        .add_permission({config::get_snowflake("owner_id"), dpp::cpt_user, true});
}

bool ReloadCmd::run(const dpp::interaction_create_t& event) {
    config::reset();
    config::load("./config.json");

    inventory::reset();
    inventory::load(config::get_string("boh_path"));

    characters::reset();
    characters::load(config::get_string("chars_folder"));

    event.reply(dpp::ir_channel_message_with_source, dpp::message("Ok").set_flags(dpp::m_ephemeral));
    return true;
}

ReloadCmd::~ReloadCmd() {}

} // namespace Kulike
