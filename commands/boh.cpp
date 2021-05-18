#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "commands.hpp"

namespace Kulike {

json read_boh() {
    json boh;
    std::ifstream bohfile("../data/boh.json");
    bohfile >> boh;

    return boh;
}

BOHCmd::BOHCmd() {
    this->key = "boh";

    dpp::command_option gold_subcommand(dpp::co_sub_command, "gold", "Ver a quantidade de ouro na Bag of Holding");
    dpp::command_option items_subcommand(dpp::co_sub_command, "items", "Vê uma lista de itens na Bag of Holding");

    this->_command.set_name(this->key)
        .set_description("Bag of Holding")
        .add_option(
            gold_subcommand
                .add_option(dpp::command_option(dpp::co_string, "operation", "Soma ou subtração")
                                .add_choice(dpp::command_option_choice("+", "plus"))
                                .add_choice(dpp::command_option_choice("-", "minus")))
                .add_option(dpp::command_option(dpp::co_integer, "value", "Valor a ser adicionado ou subtraido")))
        .add_option(items_subcommand);

    std::cout << this->_command.build_json(false) << std::endl;
}

bool BOHCmd::run(const dpp::interaction_create_t& event) {
    dpp::command_interaction cmd_data = std::get<dpp::command_interaction>(event.command.data);

    for (auto& c : cmd_data.options) {
        std::cout << c.name << " " << c.value.index() << std::endl;
    }

    event.reply(dpp::ir_channel_message_with_source, "oi");
    return true;
}

} // namespace Kulike
