#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "Kulibot.hpp"
#include "commands/commands.hpp"
#include "utils/config.hpp"

namespace Kulike {

Kulibot::Kulibot(std::string token) : dpp::cluster(token) { spdlog::info("Initializing Kulibot..."); }

void Kulibot::add_command(Command* cmd) {
    cmd->set_app_id(this->me.id);

    // If a command with the same key already exists, we need to delete it
    // to avoid a leak by simply replacing it
    if (this->commands.contains(cmd->get_key())) {
        delete this->commands[cmd->get_key()];
    }

    this->commands[cmd->get_key()] = cmd;
}

void Kulibot::setup_log() {
    this->on_log([](const dpp::log_t& event) {
        switch (event.severity) {
        case dpp::ll_trace:
            spdlog::trace(event.message);
            break;
        case dpp::ll_debug:
            spdlog::debug(event.message);
            break;
        case dpp::ll_info:
            spdlog::info(event.message);
            break;
        case dpp::ll_warning:
            spdlog::warn(event.message);
            break;
        case dpp::ll_error:
            spdlog::error(event.message);
            break;
        case dpp::ll_critical:
        default:
            spdlog::critical(event.message);
            break;
        }
    });
}

void Kulibot::setup_interactions() {
    this->on_ready([this](const dpp::ready_t& event) {
        (void)event;

        spdlog::info(fmt::format("Logged in as {}.", this->me.username));
        // TODO: This is always returning 0
        spdlog::info(fmt::format("Connected to {} guilds.", event.from->get_guild_count()));
        spdlog::info(fmt::format("Connected to {} users.", event.from->get_member_count()));

        for (auto& [key, cmd] : this->commands) {
            spdlog::info(fmt::format("Registering {}", key));

            // Registering only on test guild for now, for faster command updates
            this->guild_command_create(
                cmd->get_inner_command(), config::get_snowflake("guild_ids/thundergaming"),
                [&](const dpp::confirmation_callback_t& c) {
                    if (!std::get<dpp::confirmation>(c.value).success) {
                        spdlog::error(fmt::format("Couldn't register command {}: {}", key, c.http_info.body));
                        return;
                    }

                    spdlog::debug(fmt::format("Successfully registered command {}", key));
                });
        }
    });

    this->on_interaction_create([this](const dpp::interaction_create_t& event) {
        if (event.command.type != dpp::it_application_command) {
            spdlog::warn(fmt::format("Received wrong interaction create command type: {}", event.command.type));
            return;
        }

        dpp::command_interaction cmd_data = std::get<dpp::command_interaction>(event.command.data);

        if (this->commands.contains(cmd_data.name)) {
            spdlog::info(fmt::format("Running command {} by user {} [{}].", cmd_data.name,
                                     event.command.member.nickname, event.command.usr.id));

            if (!this->commands[cmd_data.name]->run(event)) {
                spdlog::error(fmt::format("Failed to run command {}", cmd_data.name));
            }
        } else {
            spdlog::info(fmt::format("Received unknown command: {}", cmd_data.name));
        }
    });
}

void Kulibot::setup() {
    setup_log();
    setup_interactions();
}

void Kulibot::run() { this->cluster::start(); }

Kulibot::~Kulibot() {
    spdlog::debug("Destroying kulibot...");

    for (auto& [key, cmd] : this->commands) {
        spdlog::info(fmt::format("Deleting {}", key));
        this->guild_command_delete(cmd->get_inner_command().id, config::get_snowflake("guild_ids/thundergaming"),
                                   [key](const dpp::confirmation_callback_t& c) {
                                       if (!std::get<dpp::confirmation>(c.value).success) {
                                           spdlog::error(
                                               fmt::format("Couldn't delete command {}: {}", key, c.http_info.body));
                                           return;
                                       }

                                       spdlog::debug(fmt::format("Deleted command {} [{}]", key, c.http_info.status));
                                   });

        delete cmd;
    }

    this->commands.clear();
    spdlog::debug("Done.");
}

} // namespace Kulike
