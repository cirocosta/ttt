#include "ttt/connection.hh"

#include <iostream>

using namespace ttt;

void udp_connection()
{
  net::Connection conn{ "localhost", TTT_DEFAULT_PORT,
                        net::TTT_CONNECTION_UDP_PASSIVE };
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
  net::Connection conn{ "localhost", TTT_DEFAULT_PORT,
                        net::TTT_CONNECTION_TCP_PASSIVE };
  net::ConnectionPtr client_conn;

  conn.listen();

  std::cout << "Server at " << conn.getHostname() << ":" << conn.getPort()
            << " waiting for clients" << std::endl;
  client_conn = conn.accept();
  std::cout << "client Connected: " << client_conn->getHostname() << std::endl;
}

int main(int argc, char *argv[])
{
  /* tcp_connection(); */
  udp_connection();

  return 0;
}
