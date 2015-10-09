#ifndef TTT__PROTOCOL__PARSER_HH
#define TTT__PROTOCOL__PARSER_HH

#include "ttt/protocol/msg.hh"
#include "ttt/protocol/buffer.hh"
#include "ttt/protocol/lexer.hh"

namespace ttt
{
namespace protocol
{

struct str_cmp {
  bool operator()(char const* a, char const* b) const
  {
    return std::strcmp(a, b) < 0;
  }
};

const static std::map<char const*, COMMAND, str_cmp> COMMAND_MAP = {
  { "CMD_REG", CMD_REG },
  { "CMD_IN", CMD_IN },
  { "CMD_OUT", CMD_OUT },
  { "CMD_HB", CMD_HB },
};

struct Parser {
  static Message parse_msg(std::string msg);
};
}
};

#endif
