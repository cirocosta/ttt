#include "ttt/tls_connection.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::net;

TEST(TLSConnection, SingleConnectionCreationAndDeletion)
{
  TLSConnection conn_act("localhost", 1337, TLS_ACTIVE);

  ASSERT_TRUE(conn_act.getConnection());
  ASSERT_EQ(conn_act.getConnection()->isUDP(), false);
  ASSERT_EQ(conn_act.getConnection()->isPassive(), false);
}

TEST(TLSConnection, MultipleConnectionCreationAndDeletion)
{
  TLSConnection conn_act("localhost", 1337, TLS_ACTIVE);
  TLSConnection conn_act2("localhost", 1338, TLS_ACTIVE);
  TLSConnection conn_act3("localhost", 1339, TLS_ACTIVE);
  /* TLSConnection conn_pas("localhost", 1337, TLS_PASSIVE); */

  ASSERT_EQ(conn_act.getConnection()->isUDP(), false);
  ASSERT_EQ(conn_act.getConnection()->isPassive(), false);

  ASSERT_EQ(conn_act2.getConnection()->isUDP(), false);
  ASSERT_EQ(conn_act2.getConnection()->isPassive(), false);

  ASSERT_EQ(conn_act3.getConnection()->isUDP(), false);
  ASSERT_EQ(conn_act3.getConnection()->isPassive(), false);
}

TEST(TLSConnection, LooseCreationAndDeletion)
{
  ConnectionPtr conn_tcp =
      ConnectionPtr(new Connection("localhost", 1337, TCP_ACTIVE));

  ASSERT_TRUE(conn_tcp.get());

  TLSConnection conn_tls(conn_tcp);

  ASSERT_FALSE(conn_tcp.get());
  ASSERT_TRUE(conn_tls.getConnection());

  ASSERT_EQ(conn_tls.getConnection()->isUDP(), false);
  ASSERT_EQ(conn_tls.getConnection()->isPassive(), false);
  ASSERT_EQ(conn_tls.getConnection()->getType(), TLS_ACTIVE);
}

