#include "ttt/protocol/lexer.hh"

namespace ttt
{
namespace protocol
{

static char const* _is_single_terminal(char const* la, char c)
{
  return *la == c ? ++la : NULL;
}


static char const* _is_terminal(char const* peek, char const* terminal, unsigned size)
{
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


/* int yi_lex_single_terminal(yi_buffer_t* buf, char c) */
/* { */
/*   char const* peek; */

/*   if (!(peek = _is_single_terminal(buf->la, c))) */
/*     return 0; */

/*   yi_buffer_update(buf, peek, YI_T_SINGLE_TERMINAL); */

/*   return 1; */
/* } */

}
};
