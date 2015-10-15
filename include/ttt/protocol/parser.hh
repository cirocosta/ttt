#ifndef TTT__PROTOCOL__PARSER_HH
#define TTT__PROTOCOL__PARSER_HH

#include "ttt/protocol/msg.hh"
#include "ttt/protocol/buffer.hh"
#include "ttt/protocol/lexer.hh"

#include <vector>

namespace ttt
{
namespace protocol
{

struct Parser {
  static Message parse_msg(const std::string& msg);
  static std::vector<Message> parse_msgs(const std::string& msg);
  /* static const char* parse(char const* msg, Message* message); */
};
}
};

#endif
