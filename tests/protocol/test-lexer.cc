#include "ttt/protocol/lexer.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::protocol;

TEST(Lexer, Terminal)
{
  {
    std::string msg = "\r\n";
    Buffer buf(msg);

    ASSERT_TRUE(Lexer::terminal(buf, STR_CRLF.c_str(), STR_CRLF.size()));
    ASSERT_FALSE(Lexer::terminal(buf, STR_CRLF.c_str(), STR_CRLF.size()));
  }
  {
    std::string msg = "";
    Buffer buf(msg);

    ASSERT_FALSE(Lexer::terminal(buf, STR_CRLF.c_str(), STR_CRLF.size()));
  }
}

TEST(Lexer, TerminalMulti)
{
  std::string msg = "\r\n\r\nooooo";
  Buffer buf(msg);

  ASSERT_TRUE(Lexer::terminal(buf, STR_CRLF.c_str(), STR_CRLF.size()));
  ASSERT_TRUE(Lexer::terminal(buf, STR_CRLF.c_str(), STR_CRLF.size()));
  ASSERT_FALSE(Lexer::terminal(buf, STR_CRLF.c_str(), STR_CRLF.size()));
}

TEST(Lexer, SingleTerminal)
{
  std::string msg = "message:message\r\n";
  Buffer buf(msg);

  ASSERT_TRUE(Lexer::terminal(buf, "message", strlen("message")));
  ASSERT_TRUE(Lexer::terminal(buf, ":", 1));
  ASSERT_TRUE(Lexer::terminal(buf, "message", strlen("message")));
  ASSERT_TRUE(Lexer::terminal(buf, STR_CRLF.c_str(), STR_CRLF.size()));
}

TEST(Lexer, Command)
{
  std::string msg = "CMD_REG:my_user:my_pwd\r\n";
  Buffer buf(msg);

  ASSERT_TRUE(Lexer::command(buf));
  ASSERT_EQ(0, strcmp(buf.token.buf, "CMD_REG"));
}

TEST(Lexer, Repl)
{
  std::string msg = "RPL_OK:Registration Ok!\r\n";
  Buffer buf(msg);

  ASSERT_TRUE(Lexer::reply(buf));
  ASSERT_EQ(0, strcmp(buf.token.buf, "RPL_OK"));
}
