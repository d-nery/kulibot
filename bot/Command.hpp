#if !defined(__COMMAND_HPP__)
#define __COMMAND_HPP__

#include <dpp/dpp.h>

namespace Kulike {

class Command {
  protected:
    dpp::slashcommand _command;
    std::string key;
    dpp::snowflake _id;

  public:
    virtual ~Command(){};

    dpp::slashcommand get_inner_command() const { return _command; }
    std::string get_key() const { return key; }
    dpp::snowflake get_id() const { return _id; }

    void set_app_id(dpp::snowflake i) { _command.set_application_id(i); }
    void set_id(dpp::snowflake i) { _id = i; }

    virtual bool run(const dpp::interaction_create_t& event) = 0;
};

} // namespace Kulike

#endif // __COMMAND_HPP__
