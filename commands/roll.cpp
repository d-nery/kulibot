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

    auto options = dpp::command_option(dpp::co_string, "stat", "Habilidade/Saving Throw a ser realizado", true);

    for (auto& stat : characters::stats) {
        options.add_choice(dpp::command_option_choice(stat, stat));
    }

    for (auto& [skill, _] : characters::skills) {
        options.add_choice(dpp::command_option_choice(skill, skill));
    }

    options.add_choice(dpp::command_option_choice("init", "init"));

    this->_command.set_name(this->key)
        .set_description("Roda um dado para o seu personagem")
        .add_option(options)
        .add_option(dpp::command_option(dpp::co_integer, "extra",
                                        "Algum valor adicional (ou negativo) não considerado normalmente"))
        .add_option(dpp::command_option(dpp::co_boolean, "advantage", "Vantagem"))
        .add_option(dpp::command_option(dpp::co_boolean, "disadvantage", "Desvantagem"));
    ;
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
    bool advantage = false;
    bool disadvantage = false;

    for (auto& opt : cmd_data.options) {
        if (opt.name == "stat") {
            stat = std::get<std::string>(opt.value);
        } else if (opt.name == "extra") {
            extra = std::get<int>(opt.value);
        } else if (opt.name == "advantage") {
            advantage = std::get<bool>(opt.value);
        } else if (opt.name == "disadvantage") {
            disadvantage = std::get<bool>(opt.value);
        }
    }

    int roll = c.roll();
    int discarded_roll = 0;
    int mod = c.modifier(stat);

    if (advantage) {
        int new_roll = c.roll();
        discarded_roll = std::min(roll, new_roll);
        roll = std::max(roll, new_roll);
    } else if (disadvantage) {
        int new_roll = c.roll();
        discarded_roll = std::max(roll, new_roll);
        roll = std::min(roll, new_roll);
    }

    std::string msg = config::get_string("emoji/d20") + " **" + c.name + "**";
    msg += roll == 20 ? " 🎉\n" : roll == 1 ? " 😵\n" : "\n";

    int total = std::max(roll + mod + extra, 1);
    msg += fmt::format("{}: {} {}({:+d})\n", stat, roll,
                       discarded_roll == 0 ? "" : fmt::format("~~{}~~ ", discarded_roll), mod + extra);
    msg += fmt::format("total: **{}**\n", total);

    event.reply(dpp::ir_channel_message_with_source, msg);

    return true;
}

RollCmd::~RollCmd() {}

} // namespace Kulike
