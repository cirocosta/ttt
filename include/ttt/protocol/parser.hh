#ifndef TTT__PROTOCOL__PARSER_HH
#define TTT__PROTOCOL__PARSER_HH

#include "ttt/protocol/msg.hh"
#include "ttt/protocol/buffer.hh"
#include "ttt/protocol/lexer.hh"

namespace ttt
{
namespace protocol
{


struct Parser {
  static Message parse_msg(std::string msg);
};
}
};

#endif
