#include "ttt/protocol/buffer.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::protocol;

TEST(Buffer, creation)
{
  std::string msg = "message";
  Buffer buf(msg);

  ASSERT_EQ(0, buf.line);
  ASSERT_EQ(0, buf.column);
  ASSERT_EQ(msg.size(), buf.size);
  ASSERT_EQ(msg.c_str(), buf.la);
  ASSERT_EQ(msg.c_str(), buf.buf);
}

TEST(Buffer, ColumnFeed)
{
  std::string msg = "message";
  Buffer buf(msg);

  buf.cf();
  buf.cf();
  buf.cf();

  ASSERT_EQ(3, buf.column);
  ASSERT_EQ(0, buf.line);
  ASSERT_EQ(0, strcmp(buf.la, "sage"));
  ASSERT_EQ(0, strcmp(buf.buf, "message"));
}

TEST(Buffer, LineFeed)
{
  std::string msg = "message";
  Buffer buf(msg);

  buf.cf();
  buf.cf();
  buf.cf();
  buf.lf();
  buf.cf();

  ASSERT_EQ(1, buf.column);
  ASSERT_EQ(1, buf.line);
  ASSERT_EQ(0, strcmp(buf.la, "age"));
  ASSERT_EQ(0, strcmp(buf.buf, "message"));
}

TEST(Buffer, Update)
{
  std::string msg = "message1 message2";
  Buffer buf(msg);

  const char *peek = strstr(buf.buf, "message2");

  buf.update(peek);

  ASSERT_EQ(9, buf.column);
  ASSERT_EQ(0, strcmp("message2", buf.la));
  ASSERT_EQ(0, strcmp("message1 ", buf.token.buf));
}

TEST(Buffer, Reset)
{
  std::string msg1 = "aaaaaaaa";
  std::string msg2 = "bbb";

  Buffer buf(msg1);

  buf.update(buf.la+3);
  ASSERT_EQ(3, buf.column);

  buf.reset(msg2.c_str(), msg2.size());
  ASSERT_EQ(0, strcmp(buf.la, "bbb"));
  ASSERT_EQ(0, strcmp(buf.buf, "bbb"));
  ASSERT_EQ(0, buf.column);
  ASSERT_EQ(0, buf.line);
}
