#include "ttt/protocol/lexer.hh"

namespace ttt
{
namespace protocol
{

static char const* _is_terminal(char const* peek, char const* terminal,
                                unsigned size)
{
  if (size == 1)
    return *peek == terminal[0] ? peek + 1 : NULL;

  if (strncmp(peek, terminal, size))
    return NULL;

  return peek + size;
}

bool Lexer::terminal(Buffer& buf, char const* terminal, unsigned size)
{
  char const* peek;

  if (!(peek = _is_terminal(buf.la, terminal, size)))
    return false;
  buf.update(peek);

  return true;
}

bool Lexer::command(Buffer& buf)
{
  char const* peek;

  if (!(peek = _is_terminal(buf.la, STR_CMD.c_str(), STR_CMD.size())))
    return false;

  while(peek && isalpha(*peek))
    peek++;
  buf.update(peek);

  return true;
}

bool Lexer::reply(Buffer& buf)
{
  char const* peek;

  if (!(peek = _is_terminal(buf.la, STR_RPL.c_str(), STR_RPL.size())))
    return false;

  while(peek && isalpha(*peek))
    peek++;
  buf.update(peek);

  return true;
}
}
};
