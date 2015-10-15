#ifndef TTT__PROTOCOL__MSG_HH
#define TTT__PROTOCOL__MSG_HH

#include "ttt/constants.hh"
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <initializer_list>

namespace ttt
{

namespace protocol
{
struct Message {
  COMMAND command;
  std::vector<std::string> args;

  Message(COMMAND c, std::vector<std::string> a) : command(c), args(a) {}

  std::string toString() const
  {
    std::string str = std::accumulate(
        args.begin(), args.end(), COMMAND_INV_MAP.find(command)->second + ':',
        [](const std::string& a, const std::string& b) { return a + b + ':'; });

    return str + "\r\n";
  }

  inline static std::string str(COMMAND cmd,
                                std::initializer_list<std::string> args)
  {
    return Message{ cmd, std::vector<std::string>(args) }.toString();
  }
};
}
};
#endif
