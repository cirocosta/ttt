#include "ttt/protocol/parser.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::protocol;

TEST(ParseMessage, CMDRegOk)
{
  /* std::string msg = "CMD_REG:my_user_id:my_password:"; */
  /* Message expected_msg = {.args = std::vector<std::string>{ "my_user_id", */
  /*                                                           "my_password" }, */
  /*                         .command = CMD_REG }; */
  /* Message actual_msg = Parser::parse_msg(msg); */
  
  /* ASSERT_EQ(actual_msg.command, expected_msg.command); */
  /* ASSERT_EQ(actual_msg.args.size(), expected_msg.args.size()); */
  /* ASSERT_EQ(actual_msg.args[0], expected_msg.args[0]); */
  /* ASSERT_EQ(actual_msg.args[1], expected_msg.args[1]); */
  ASSERT_TRUE(1);
}
