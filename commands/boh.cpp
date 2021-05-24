#include <sstream>

#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "commands.hpp"

#include "utils/config.hpp"
#include "utils/inventory.hpp"

namespace Kulike {

static const std::string config_key("boh_path");

BOHCmd::BOHCmd() {
    this->key = "boh";

    inventory::load(config::get_string(config_key));

    dpp::command_option gold_subcommand(dpp::co_sub_command, "gold", "Adicionar (ou remover) ouro na Bag of Holding");
    dpp::command_option items_subcommand(dpp::co_sub_command, "list", "Vê uma lista de itens na Bag of Holding");

    dpp::command_option categories_options(dpp::co_string, "category", "Categoria de item a ser listada", true);

    for (auto cat : inventory::get_categories()) {
        categories_options.add_choice(dpp::command_option_choice(cat, cat));
    }

    this->_command.set_name(this->key)
        .set_description("Bag of Holding")
        .add_option(gold_subcommand.add_option(
            dpp::command_option(dpp::co_integer, "value", "Valor a ser adicionado ou subtraido")))
        .add_option(items_subcommand);
}

bool BOHCmd::run(const dpp::interaction_create_t& event) {
    dpp::command_interaction cmd_data = std::get<dpp::command_interaction>(event.command.data);

    auto& subc = cmd_data.options[0];

    spdlog::info(fmt::format("Received BOHCmd {}", subc.name));

    if (subc.name == "gold") {
        if (subc.options.size() > 0) {
            int val = std::get<int>(subc.options[0].value);

            inventory::add_gold(val);
            inventory::save(config::get_string(config_key));

            event.reply(
                dpp::ir_channel_message_with_source,
                fmt::format("Feito! Ouro atual: {}: {}", config::get_string("emoji/gold_coin"), inventory::get_gold()));
        } else {
            event.reply(dpp::ir_channel_message_with_source,
                        fmt::format("{}: {}", config::get_string("emoji/gold_coin"), inventory::get_gold()));
        }
    } else if (subc.name == "list") {
        dpp::embed embed;
        embed.set_color(0xA7A325).set_title("Bag of Holding");
        embed.set_thumbnail(config::get_string("boh_icon"));
        embed.footer = {"Kulikê de Obsidiana",
                        "https://cdn.discordapp.com/attachments/637477278476927006/846208361866592286/"
                        "7bc57694dfd9d730756198cb26df228d.png",
                        ""};
        embed.timestamp = std::time(0);

        embed.add_field(
            "Gold", fmt::format("{} {}", config::get_string("emoji/gold_coin"), std::to_string(inventory::get_gold())));

        std::ostringstream oss;

        for (auto cat : inventory::get_categories()) {
            auto items = inventory::get_items(cat);

            for (auto& item : items) {
                oss << fmt::format("{}x {}\n", item.amount, item.name);
            }

            embed.add_field(cat, oss.str(), true);
            oss.str("");
            oss.clear();
        }

        dpp::message msg(event.command.channel_id, embed);

        event.reply(dpp::ir_channel_message_with_source, msg);
    } else {
        spdlog::warn(fmt::format("Unknown BOH sub-command: {}", subc.name));
        return false;
    }

    return true;
}

BOHCmd::~BOHCmd() { spdlog::debug("Destroying BOHCmd"); }

} // namespace Kulike
