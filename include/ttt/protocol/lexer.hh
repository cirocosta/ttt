#ifndef TTT__PROTOCOL__LEXER_HH
#define TTT__PROTOCOL__LEXER_HH

#include "ttt/protocol/buffer.hh"


namespace ttt
{
namespace protocol
{

static const std::string STR_CRLF = "\r\n";
static const std::string STR_COLON = ":";
static const std::string STR_CMD = "CMD_";
static const std::string STR_RPL = "RPL_";


struct Lexer {
  /**
   * Returns a boolean in case it can lex properly.
   * In case of success:
   *  - updates the buffer
   * Else:
   *  - returns false and keeps the buffer as it
   */
  static bool terminal(Buffer& buf, char const* terminal, unsigned size);
  static bool command(Buffer& buf);
  static bool reply(Buffer& buf);
};
}
};

#endif
