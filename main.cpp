/**
 * @file main.cpp
 * @brief Main file for the kulibot
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#include <csignal>
#include <iostream>

#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "bot/Kulibot.hpp"
#include "commands/commands.hpp"

#include "utils/config.hpp"
#include "utils/inventory.hpp"

using namespace Kulike;

int main() {
    spdlog::set_level(spdlog::level::level_enum::debug);
    spdlog::info("[main] Starting...");

    if (!config::load("./config.json")) {
        spdlog::error("[main] Failed to load config, aborting...");
        std::exit(1);
    }

    Kulike::Kulibot bot(config::get_string("token"));

    // All commands are deleted on bot destruction
    bot.add_command(new Kulike::PingCmd());
    bot.add_command(new Kulike::BOHCmd());
    bot.add_command(new Kulike::CharCmd());
    bot.add_command(new Kulike::RollCmd());
    bot.add_command(new Kulike::ReloadCmd());

    bot.setup();

    bot.run();

    spdlog::info("[main] Quitting...");
    return 0;
}
