#include "ttt/protocol/lexer.hh"

namespace ttt
{
namespace protocol
{

inline static char const* _is_any_except(char const* peek,
                                         const char* exceptions, unsigned size)
{
  while (size-- > 0)
    if (*peek == exceptions[size])
      return NULL;

  return peek + 1;
}

inline static char const* _is_in_range(char const* peek, char begin, char end)
{
  if (!(*peek >= begin && *peek < end))
    return NULL;

  return peek + 1;
}

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

bool Lexer::arg(Buffer& buf)
{
  char const* peek;
  char const* tmp_peek;

  if (!(tmp_peek = _is_any_except(buf.la, "\0:", 2)))
    return false;


  while ((tmp_peek = _is_any_except(tmp_peek, "\0:", 2)))
    peek = tmp_peek;

  buf.update(peek);

  return true;
}

bool Lexer::command(Buffer& buf)
{
  char const* peek;

  if (!(peek = _is_terminal(buf.la, STR_CMD.c_str(), STR_CMD.size())))
    return false;

  while (peek && isalpha(*peek))
    peek++;
  buf.update(peek);

  return true;
}

bool Lexer::reply(Buffer& buf)
{
  char const* peek;

  if (!(peek = _is_terminal(buf.la, STR_RPL.c_str(), STR_RPL.size())))
    return false;

  while (peek && isalpha(*peek))
    peek++;
  buf.update(peek);

  return true;
}
}
};
