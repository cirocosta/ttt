#include "ttt/protocol/parser.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::protocol;

TEST(ParseMessage, CMDRegOk)
{
  std::string msg = "CMD_REG:my_user_id:my_password:";
  Message expected_msg = {.args = std::vector<std::string>{ "my_user_id",
                                                            "my_password" },
                          .command = CMD_REG };
  Message actual_msg = Parser::parse_msg(msg);

  ASSERT_EQ(actual_msg.command, expected_msg.command);
  ASSERT_EQ(actual_msg.args.size(), expected_msg.args.size());
  ASSERT_EQ(actual_msg.args[0], expected_msg.args[0]);
  ASSERT_EQ(actual_msg.args[1], expected_msg.args[1]);
}

TEST(ParseMessage, MessageWithCRLF)
{
  std::string msg = "CMD_REG:user1:my_password:\r\n";
  Message expected_msg = {.args = std::vector<std::string>{ "user1",
                                                            "my_password" },
                          .command = CMD_REG };
  Message actual_msg = Parser::parse_msg(msg);

  ASSERT_EQ(actual_msg.command, expected_msg.command);
  ASSERT_EQ(actual_msg.args.size(), expected_msg.args.size());
  ASSERT_EQ(actual_msg.args[0], expected_msg.args[0]);
  ASSERT_EQ(actual_msg.args[1], expected_msg.args[1]);
}

/* TEST(ParseMessage, MultipleMessages) */
/* { */
/*   Message msg1; */
/*   Message msg2; */
/*   char const* msg = "CMD_REG:user1:my_password:\r\n" */
/*                     "CMD_REG:user2:my_password:\r\n"; */

/*   char const* curr = Parser::parse(msg, &msg1); */
/*   curr = Parser::parse(curr, &msg2); */

/*   ASSERT_EQ("user1", msg1.args[0]); */
/*   ASSERT_EQ("user2", msg2.args[0]); */
/* } */
