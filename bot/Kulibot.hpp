/**
 * @file Kulibot.hpp
 * @brief Main bot class
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#if !defined(__KULIBOT_HPP__)
#define __KULIBOT_HPP__

#include <dpp/dpp.h>

#include "Command.hpp"

namespace Kulike {

/**
 * @brief Main bot class, derives from dpp::cluster
 *        adding a command handler
 */
class Kulibot : public dpp::cluster {
  private:
    std::unordered_map<std::string, Command*> commands; //!< Command map

    /**
     * @brief Sets up application/dpp log
     *
     * @note Sets on_log callback
     */
    void setup_log();

    /**
     * @brief setup slash commands
     *
     * @note Sets on_ready and on_interaction_create callbacks
     */
    void setup_interactions();

  public:
    /**
     * @brief Constructs the cluster
     *
     * @param token Discord bot token
     */
    Kulibot(std::string token);
    ~Kulibot();

    /**
     * @brief Add a new command to the bot, commands with existing keys are replaced
     *
     * @param cmd Command to be added
     */
    void add_command(Command* cmd);

    /**
     * @brief Set up the bot, should be called after all commands are added
     */
    void setup();

    /**
     * @brief Runs the bot, must have added commands and run setup.
     *
     * @note This blocks
     */
    void run();
};

} // namespace Kulike

#endif // __KULIBOT_HPP__
