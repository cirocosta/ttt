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
  static bool terminal(Buffer& buf, char const* terminal, unsigned size);
  static bool command(Buffer& buf);
  static bool arg(Buffer& buf);
  static bool crlf(Buffer& buf);
  static inline bool colon(Buffer& buf) { return Lexer::terminal(buf, ":", 1); }
};
}
};

#endif
