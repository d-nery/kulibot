#include <csignal>
#include <iostream>

#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "bot/Kulibot.hpp"
#include "commands/commands.hpp"

std::atomic<bool> should_quit(false);

int main() {
    std::cout << "Starting..." << std::endl;

    json configdocument;

    {
        std::ifstream configfile("./config.json");
        configfile >> configdocument;
    }

    Kulike::Kulibot bot(configdocument["token"]);

    bot.add_command(new Kulike::PingCmd());
    bot.add_command(new Kulike::BOHCmd());

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
