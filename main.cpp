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

std::atomic<bool> should_quit(false);

int main() {
    std::cout << "Starting..." << std::endl;

    config::load("./config.json");

    Kulike::Kulibot bot(config::get_string("token"));

    // All commands are deleted on bot destruction
    bot.add_command(new Kulike::PingCmd());
    bot.add_command(new Kulike::BOHCmd());
    bot.add_command(new Kulike::CharCmd());

    bot.setup();

    signal(SIGINT, [](int sig_num) {
        (void)sig_num;

        std::cout << "Received SIGINT, stopping..." << std::endl;
        should_quit.store(true);
    });

    bot.run();

    while (!should_quit.load()) {
    }

    std::cout << "Quitting..." << std::endl;
    // std::terminate();
    return 0;
}
