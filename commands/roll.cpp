/**
 * @file roll.cpp
 * @brief Roll dices for characters
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#include <dpp/dpp.h>
#include <spdlog/spdlog.h>

#include "commands.hpp"
#include "utils/characters.hpp"
#include "utils/config.hpp"

namespace Kulike {

static const std::string config_key("chars_folder");

RollCmd::RollCmd() {
    this->key = "roll";

    // CharCmd also does this, just make sure it is done, doing again won't hurt
    // as the helper method refuses to load again
    characters::load(config::get_string(config_key));

    this->_command.set_name(this->key)
        .set_description("Roda um dado para o seu personagem")
        .add_option(dpp::command_option(dpp::co_string, "stat",
                                        "Habilidade/Saving Throw a ser realizado, em minúsculo e inglês", true))
        .add_option(dpp::command_option(dpp::co_integer, "extra",
                                        "Algum valor adicional (ou negativo) não considerado normalmente"));
}

bool RollCmd::run(const dpp::interaction_create_t& event) {
    uint64_t user_id = event.command.usr.id;

    auto c = characters::get(user_id);

    if (c.name == "") {
        spdlog::error(fmt::format("Couldn't find char for owner {}", user_id));
        event.reply(dpp::ir_channel_message_with_source, "Não achei um personagem ligado a você! 😢");
        return false;
    }

    dpp::command_interaction cmd_data = std::get<dpp::command_interaction>(event.command.data);

    std::string stat;
    int extra = 0;

    for (auto& opt : cmd_data.options) {
        if (opt.name == "stat") {
            stat = std::get<std::string>(opt.value);
        } else if (opt.name == "extra") {
            extra = std::get<int>(opt.value);
        }
    }

    uint32_t roll = c.roll(stat);
    roll += extra;

    event.reply(dpp::ir_channel_message_with_source,
                fmt::format("{} {} ({:+d}) para {}: {}", config::get_string("emoji/d20"), stat,
                            c.modifier(stat) + extra, c.name, roll));

    return true;
}

RollCmd::~RollCmd() {}

} // namespace Kulike
