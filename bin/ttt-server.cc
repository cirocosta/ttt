#include "ttt/connection.hh"

#include <iostream>

using namespace ttt;

int main(int argc, char *argv[])
{
  net::Connection conn{ "localhost", 8080, net::TTT_CONNECTION_TCP_PASSIVE };
  net::ConnectionPtr client_conn;

  conn.listen();

  std::cout << "Server at " << conn.getHostname() << ":" << conn.getPort()
            << " waiting for clients" << std::endl;
  client_conn = conn.accept();
  std::cout << "client Connected: " << client_conn->getHostname() << std::endl;
  /* client_conn->writeTo("Hello client!"); */
  /* client_conn->close(); */

  return 0;
}
