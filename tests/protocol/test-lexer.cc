#include "ttt/protocol/lexer.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::protocol;

TEST(Lexer, Terminal)
{
  {
    std::string msg = "\r\n";
    Buffer buf(msg);

    ASSERT_TRUE(Lexer::terminal(buf, CRLF, strlen(CRLF)));
    ASSERT_FALSE(Lexer::terminal(buf, CRLF, strlen(CRLF)));
  }
  {
    std::string msg = "";
    Buffer buf(msg);

    ASSERT_FALSE(Lexer::terminal(buf, CRLF, strlen(CRLF)));
  }
}

TEST(Lexer, TerminalMulti)
{
  std::string msg = "\r\n\r\nooooo";
  Buffer buf(msg);

  ASSERT_TRUE(Lexer::terminal(buf, CRLF, strlen(CRLF)));
  ASSERT_TRUE(Lexer::terminal(buf, CRLF, strlen(CRLF)));
  ASSERT_FALSE(Lexer::terminal(buf, CRLF, strlen(CRLF)));
}
