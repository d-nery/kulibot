/**
 * @file Command.hpp
 * @brief Generic command definitions
 *
 * @author Daniel Nery <danielnso97@gmail.com>
 * @date 05/2021
 */

#if !defined(__COMMAND_HPP__)
#define __COMMAND_HPP__

#include <dpp/dpp.h>

namespace Kulike {

/**
 * @brief Abstract command class, these are added to the bot
 */
class Command {
  protected:
    dpp::slashcommand _command; //!< Underlying slash command
    std::string key;            //!< Command key, must be unique in the bot
    dpp::snowflake _id;         //!< Command ID as returned by discord

  public:
    virtual ~Command(){};

    /**
     * @brief Getters
     */
    dpp::slashcommand& get_inner_command() { return _command; }
    std::string get_key() const { return key; }
    dpp::snowflake get_id() const { return _id; }

    /**
     * @brief Setters
     */
    void set_app_id(dpp::snowflake i) { _command.set_application_id(i); }
    void set_id(dpp::snowflake i) { _id = i; }

    /**
     * @brief Run the command, this method is overriden on each command
     */
    virtual bool run(const dpp::interaction_create_t& event) = 0;
};

} // namespace Kulike

#endif // __COMMAND_HPP__
