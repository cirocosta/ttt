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

TEST(Lexer, Colon)
{
  std::string msg = "\r\n:\r\n";
  Buffer buf(msg);

  ASSERT_TRUE(Lexer::crlf(buf));
  ASSERT_TRUE(Lexer::colon(buf));
  ASSERT_EQ(1, buf.line);
  ASSERT_EQ(1, buf.column);
  ASSERT_TRUE(Lexer::crlf(buf));
  ASSERT_EQ(2, buf.line);
  ASSERT_EQ(0, buf.column);
}

TEST(Lexer, CRLF)
{
  std::string msg = "\r\n\r\nooooo";
  Buffer buf(msg);

  ASSERT_TRUE(Lexer::crlf(buf));
  ASSERT_TRUE(Lexer::crlf(buf));
  ASSERT_EQ(2, buf.line);
  ASSERT_FALSE(Lexer::terminal(buf, STR_CRLF.c_str(), STR_CRLF.size()));
}

TEST(Lexer, SingleTerminal)
{
  std::string msg = "message:message\r\n";
  Buffer buf(msg);

  ASSERT_TRUE(Lexer::terminal(buf, "message", strlen("message")));
  ASSERT_TRUE(Lexer::colon(buf));
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

TEST(Lexer, Arg)
{
  std::string msg = "RPL_OK:Registration Ok!:\r\n";
  Buffer buf(msg);

  ASSERT_TRUE(Lexer::arg(buf));
  ASSERT_EQ(0, strcmp(buf.token.buf, "RPL_OK"));

  ASSERT_TRUE(Lexer::colon(buf));

  ASSERT_TRUE(Lexer::arg(buf));
  ASSERT_EQ(0, strcmp(buf.token.buf, "Registration Ok!"));

  ASSERT_TRUE(Lexer::colon(buf));
  ASSERT_FALSE(Lexer::arg(buf));

  ASSERT_TRUE(Lexer::terminal(buf, STR_CRLF.c_str(), STR_CRLF.size()));
}

