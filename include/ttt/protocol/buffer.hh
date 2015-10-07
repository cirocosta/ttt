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
};

struct Buffer {
  char const* la;  // curr post
  const char* buf; // initial
  unsigned column; // pos in line
  unsigned line;   // line in line
  unsigned size;   // buf size
  Token token;

  Buffer(std::string msg) { reset(msg.c_str(), msg.size()); }

  // line feed
  inline void lf()
  {
    line++;
    column = 0;
  }

  // column feed
  inline void cf(unsigned incr=1)
  {
    column+=incr;
    la+=incr;
  }

  void update(char const* peek)
  {
    token.len = peek - la;
    column += token.len;
    memcpy(token.buf, la, token.len);
    token.buf[token.len] = '\0';
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
