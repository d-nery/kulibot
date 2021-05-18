#if !defined(__KULIBOT_HPP__)
#define __KULIBOT_HPP__

#include <dpp/dpp.h>

#include "Command.hpp"

namespace Kulike {

class Kulibot : public dpp::cluster {
  private:
    std::unordered_map<std::string, Command*> commands;

    void setup_log();
    void setup_interactions();

  public:
    Kulibot(std::string token);
    ~Kulibot();

    void add_command(Command* cmd);

    void setup();
    void run();
};

} // namespace Kulike

#endif // __KULIBOT_HPP__
