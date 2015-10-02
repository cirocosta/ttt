#include "ttt/connection.hh"
#include "ttt/tls_connection.hh"

#include <iostream>

using namespace ttt;

void udp_connection()
{
  net::Connection conn{ "localhost", TTT_DEFAULT_PORT, net::UDP_PASSIVE };
  net::ConnectionPtr client_conn;
  char buf[1024] = { 0 };
  int n;
  socklen_t len;
  struct sockaddr cliaddr;

  conn.listen();
  while (1) {
    n = recvfrom(conn.getSocket(), buf, 1024, 0, &cliaddr, &len);
    LOGERR("SERVER\t received: %s", buf);
    sendto(conn.getSocket(), buf, 1024, 0, &cliaddr, len);
  }
}

void tcp_connection()
{
  net::Connection conn{ "localhost", TTT_DEFAULT_PORT, net::TCP_PASSIVE };
  net::ConnectionPtr client_conn;

  conn.listen();

  std::cout << "Server at " << conn.getHostname() << ":" << conn.getPort()
            << " waiting for clients" << std::endl;
  client_conn = conn.accept();
  std::cout << "client Connected: " << client_conn->getHostname() << std::endl;

  while (client_conn->read() > 0) {
    LOGERR("Server received: %s", client_conn->getBuffer());
  }
}

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

// TEST ONLY
int main(int argc, char *argv[])
{
  /* tcp_connection(); */
  /* udp_connection(); */
  tls_connection();

  return 0;
}
