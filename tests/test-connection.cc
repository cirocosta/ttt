#include "ttt/connection.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::net;

// connection types
/* UDP_ACTIVE, */
/* UDP_PASSIVE, */
/* TCP_ACTIVE, */
/* TCP_PASSIVE, */
/* TLS_ACTIVE, */
/* TLS_PASSIVE, */

TEST(Connection, CreationAndDeletion)
{
  {
    Connection conn_act("localhost", 1337, UDP_ACTIVE);
    Connection conn_pas("localhost", 1337, UDP_PASSIVE);

    ASSERT_EQ(conn_act.isUDP(), true);
    ASSERT_EQ(conn_act.isPassive(), false);

    ASSERT_EQ(conn_pas.isUDP(), true);
    ASSERT_EQ(conn_pas.isPassive(), true);
  }

  {
    Connection conn_act("localhost", 1337, TCP_ACTIVE);
    Connection conn_pas("localhost", 1337, TCP_PASSIVE);

    ASSERT_EQ(conn_act.isUDP(), false);
    ASSERT_EQ(conn_act.isPassive(), false);

    ASSERT_EQ(conn_pas.isUDP(), false);
    ASSERT_EQ(conn_pas.isPassive(), true);
  }

  {
    Connection conn_act("localhost", 1337, TLS_ACTIVE);
    Connection conn_pas("localhost", 1337, TLS_PASSIVE);

    ASSERT_EQ(conn_act.isUDP(), false);
    ASSERT_EQ(conn_act.isPassive(), false);

    ASSERT_EQ(conn_pas.isUDP(), false);
    ASSERT_EQ(conn_pas.isPassive(), true);
  }
}
