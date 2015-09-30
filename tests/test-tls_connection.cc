#include "ttt/connection.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::net;

TEST(TLSConnection, CreationAndDeletion)
{
  Connection conn_act("localhost", 1337, TLS_ACTIVE);
  Connection conn_pas("localhost", 1337, TLS_PASSIVE);

  ASSERT_EQ(conn_act.isUDP(), false);
  ASSERT_EQ(conn_act.isPassive(), false);

  ASSERT_EQ(conn_pas.isUDP(), false);
  ASSERT_EQ(conn_pas.isPassive(), true);
}

