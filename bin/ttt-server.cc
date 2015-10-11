#include "ttt/server.hh"
#include <iostream>

#define MAXEVENTS 10

using namespace ttt;

#if 0
void tls_connection()
{
  net::TLSConnection conn{ "localhost", TTT_DEFAULT_PORT, net::TLS_PASSIVE };
  net::TLSConnectionPtr client_conn;

  conn.getConnection()->listen();

  // TODO provide a display
  LOGERR("Server at %s:%u waiting for clients",
         conn.getConnection()->getHostname().c_str(),
         conn.getConnection()->getPort());

  client_conn = conn.accept();
  LOGERR("Client connected: %s",
         client_conn->getConnection()->getHostname().c_str());

  while (client_conn->read() > 0) {
    LOGERR("Server received: %s", client_conn->getConnection()->getBuffer());
  }
}
#endif

// TEST ONLY
int main(int argc, char *argv[])
{
  Server server;

  server.initTcpListener();

  return 0;
}
