#include "ttt/protocol/parser.hh"

#include <iostream>

namespace ttt
{
namespace protocol
{

COMMAND _CMD(Buffer& buf)
{
  COMMAND cmd = CMD_UNDEFINED;
  bool is_rpl = false;

  if (!((Lexer::command(buf) || Lexer::reply(buf)) && Lexer::colon(buf)))
    throw std::logic_error("Error parsing command");

  const auto cmd_found = COMMAND_MAP.find(buf.token.buf);
  if (cmd_found != COMMAND_MAP.end())
    return cmd_found->second;

  return cmd;
}

std::vector<std::string> _ARGS(Buffer& buf)
{
  std::vector<std::string> args;

  while (Lexer::arg(buf)) {
    Lexer::terminal(buf, ":", 1);
    args.emplace_back(buf.token.buf);
  }

  return args;
}

Message Parser::parse_msg(std::string msg)
{
  Buffer buf(msg);

  COMMAND cmd = _CMD(buf);
  std::vector<std::string> args = _ARGS(buf);
  Lexer::terminal(buf, STR_CRLF.c_str(), STR_CRLF.size());

  return Message{.command = cmd, .args = args };
}
}
}
