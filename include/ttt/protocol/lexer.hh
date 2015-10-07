#ifndef TTT__PROTOCOL__LEXER_HH
#define TTT__PROTOCOL__LEXER_HH

#include "ttt/protocol/buffer.hh"

static const char* CRLF = "\r\n";
static const char* COLON = ":";
static const char* CMD_ = "CMD_";
static const char* RPL_ = "RPL_";

namespace ttt
{
namespace protocol
{
struct Lexer {
  /**
   * Returns a boolean in case it can lex properly.
   * In case of success:
   *  - updates the buffer
   * Else:
   *  - returns false and keeps the buffer as it
   */
  static bool terminal(Buffer& buf, char const* terminal, unsigned size);
  /* static bool command(Buffer* buf); */
  /* static bool reply(Buffer* buf); */
};
}
};

#endif
