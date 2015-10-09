#ifndef TTT__PROTOCOL__BUFFER_HH
#define TTT__PROTOCOL__BUFFER_HH

#include <cstring>
#include <string>
#include "ttt/constants.hh"

namespace ttt
{
namespace protocol
{
struct Token {
  char buf[TTT_MAX_MESSAGE_SIZE];
  unsigned len;

  Token () {
    memset(buf, '\0', TTT_MAX_MESSAGE_SIZE);
    len = 0;
  }
};

struct Buffer {
  char const* la;  // curr post
  const char* buf; // initial
  unsigned column; // pos in line
  unsigned line;   // line in line
  unsigned size;   // buf size
  Token token;

  Buffer(std::string msg) { reset(msg.c_str(), msg.size()); }

  void update(char const* peek)
  {
    token.len = peek - la;
    column += token.len;
    memcpy(token.buf, la, token.len);
    token.buf[token.len] = '\0';
    la = peek;
  }

  inline void soft_update(char const* peek)
  {
    column += peek - la;
    la = peek;
  }

  void reset(const char* str, unsigned len)
  {
    buf = str;
    la = buf;
    column = 0;
    line = 0;
    size = len;
  }
};
}
}

#endif
