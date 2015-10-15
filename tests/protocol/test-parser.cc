#include "ttt/protocol/parser.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::protocol;

/* TEST(ParseMessage, CMDRegOk) */
/* { */
/*   std::string msg = "CMD_REG:my_user_id:my_password:"; */
/*   Message expected_msg = { CMD_REG, std::vector<std::string>{ "my_user_id", */
/*                                                               "my_password" } }; */
/*   Message actual_msg = Parser::parse_msg(msg); */

/*   ASSERT_EQ(actual_msg.command, expected_msg.command); */
/*   ASSERT_EQ(actual_msg.args.size(), expected_msg.args.size()); */
/*   ASSERT_EQ(actual_msg.args[0], expected_msg.args[0]); */
/*   ASSERT_EQ(actual_msg.args[1], expected_msg.args[1]); */
/* } */

/* TEST(ParseMessage, MessageWithCRLF) */
/* { */
/*   std::string msg = "CMD_REG:user1:my_password:\r\n"; */
/*   Message expected_msg = { CMD_REG, */
/*                            std::vector<std::string>{ "user1", "my_password" } }; */
/*   Message actual_msg = Parser::parse_msg(msg); */

/*   ASSERT_EQ(actual_msg.command, expected_msg.command); */
/*   ASSERT_EQ(actual_msg.args.size(), expected_msg.args.size()); */
/*   ASSERT_EQ(actual_msg.args[0], expected_msg.args[0]); */
/*   ASSERT_EQ(actual_msg.args[1], expected_msg.args[1]); */
/* } */

TEST(ParseMessage, MultipleMessages)
{
  std::string src = "CMD_REG:user1:pwd1:\r\n"
                    "CMD_REG:user2:pwd2:\r\n"
                    "RPL_OK:Welcome!:0:\r\n";
  std::vector<Message> msgs = Parser::parse_msgs(src);

  ASSERT_EQ(3, msgs.size());
  /* ASSERT_EQ("user1", msgs[0].args[0]); */
  /* ASSERT_EQ("user2", msgs[1].args[0]); */
  /* ASSERT_EQ("Welcome!", msgs[2].args[0]); */
}
