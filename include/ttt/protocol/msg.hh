#ifndef TTT__PROTOCOL__MSG_HH
#define TTT__PROTOCOL__MSG_HH

#include "ttt/constants.hh"
#include <vector>
#include <string>

namespace ttt
{

namespace protocol
{
struct Message {
  COMMAND command;
  std::vector<std::string> args;
};
}
};
#endif
