#include "ttt/protocol/msg.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::protocol;

TEST(Msg, ToString)
{
  Message msg{ CMD_REG, std::vector<std::string>{ "ciro", "costa" } };

  ASSERT_EQ("CMD_REG:ciro:costa:", msg.toString());
}

TEST(Msg, str)
{
  ASSERT_EQ("CMD_REG:ciro:costa:", Message::str(CMD_REG, { "ciro", "costa" }));
}
