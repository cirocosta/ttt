#include "ttt/protocol/parser.hh"

namespace ttt
{
namespace protocol
{

Message Parser::parse_msg(std::string msg)
{
  std::vector<std::string> args;

  return Message{.command = CMD_REG, .args = args };
}

void _CMD(Buffer* buf)
{
}

void _ARGS(Buffer* buf)
{
}

void _MESSAGE(Buffer* buf) {
  _CMD(buf);
  _ARGS(buf);
  Lexer::terminal(buf, CRLF); 
}

}
}
