/**
 * @file char.cpp
 * @brief Character related commands, shows characters stats
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#include <set>
#include <sstream>

#include <dpp/dpp.h>
#include <spdlog/spdlog.h>

#include "commands.hpp"
#include "utils/characters.hpp"
#include "utils/config.hpp"

namespace Kulike {

static const std::string config_key("chars_folder");

auto joinVector = [](std::vector<std::string>& v, std::string delim) {
    // Removing duplicates
    std::set s(v.begin(), v.end());
    std::ostringstream oss;

    std::string d = "";

    for (auto& cls : s) {
        oss << d << cls;
        d = delim;
    }

    return oss.str();
};

CharCmd::CharCmd() {
    this->key = "char";
    characters::load(config::get_string(config_key));

    dpp::command_option char_option(dpp::co_string, "name", "Personagem a ser listado", true);

    for (auto& key : characters::list()) {
        auto c = characters::get(key);

        char_option.add_choice(dpp::command_option_choice(c.name, key));
    }

    this->_command.set_name(this->key).set_description("Personagens").add_option(char_option);
}

bool CharCmd::run(const dpp::interaction_create_t& event) {
    dpp::command_interaction cmd_data = std::get<dpp::command_interaction>(event.command.data);

    auto char_key = std::get<std::string>(cmd_data.options[0].value);

    spdlog::info(fmt::format("Received CharCmd {}", char_key));

    auto c = characters::get(char_key);

    dpp::embed embed;
    embed.set_title(c.name).set_color(0x6632a8).set_thumbnail(c.image);
    embed.timestamp = std::time(0);
    embed.footer = {config::get_string("embed/footer/name"), config::get_string("embed/footer/icon"), ""};

    embed.add_field("Race", c.race, true)
        .add_field("Classes", joinVector(c.classes, "\n"), true)
        .add_field("\u200B", "\u200B", true)
        .add_field("Background", c.background, true)
        .add_field("Alignment", c.alignment, true)
        .add_field("\u200B", "\u200B", true)
        .add_field("STR", fmt::format("{} ({:+d})", c.stats["str"], c.modifier("str")), true)
        .add_field("DEX", fmt::format("{} ({:+d})", c.stats["dex"], c.modifier("dex")), true)
        .add_field("CON", fmt::format("{} ({:+d})", c.stats["con"], c.modifier("con")), true)
        .add_field("INT", fmt::format("{} ({:+d})", c.stats["int"], c.modifier("int")), true)
        .add_field("WIS", fmt::format("{} ({:+d})", c.stats["wis"], c.modifier("wis")), true)
        .add_field("CHA", fmt::format("{} ({:+d})", c.stats["cha"], c.modifier("cha")), true)
        .add_field("\u200B", "\u200B", false)
        .add_field("Proficiencies", joinVector(c.proficiencies, ", "), false)
        .add_field("MAX HP", std::to_string(c.max_hp), true)
        .add_field("AC", std::to_string(c.ac), true);

    dpp::message msg(event.command.channel_id, embed);

    event.reply(dpp::ir_channel_message_with_source, msg);

    return true;
}

CharCmd::~CharCmd() {}

} // namespace Kulike
