#include "ttt/tls_connection.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::net;

TEST(TLSConnection, CreationAndDeletion)
{
  TLSConnection::initialize_TLS();
  TLSConnection conn_act("localhost", 1337, TLS_ACTIVE);
  TLSConnection conn_pas("localhost", 1337, TLS_PASSIVE);

  ASSERT_EQ(conn_act.getConnection()->isUDP(), false);
  ASSERT_EQ(conn_act.getConnection()->isPassive(), false);
  ASSERT_EQ(conn_pas.getConnection()->isUDP(), false);
  ASSERT_EQ(conn_pas.getConnection()->isPassive(), true);
}

TEST(TLSConnection, LooseCreationAndDeletion)
{
  TLSConnection::initialize_TLS();
  ConnectionPtr conn_tcp =
      ConnectionPtr(new Connection("localhost", 1337, TCP_ACTIVE));
  TLSConnection conn_tls(conn_tcp);

  ASSERT_EQ(conn_tls.getConnection()->isUDP(), false);
  ASSERT_EQ(conn_tls.getConnection()->isPassive(), false);
  ASSERT_EQ(conn_tls.getConnection()->getType(), TLS_ACTIVE);
}
