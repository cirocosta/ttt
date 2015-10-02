#include "ttt/connection.hh"
#include "ttt/tls_connection.hh"

#include <string>
#include <iostream>

using namespace ttt;

const static char* CLI_HELP =
    "ttt-client\n"
    "\tInitiates a tic-tac-toe(ttt) client.\n"
    "\tIt connects itself to the port 1337 the provided\n"
    "\tserver address.\n"
    "\n"
    "Usage: \n"
    "\t./ttt-client <address> [-u]\n"
    "\n"
    "Parameters:\n."
    "\t-u\tUse UDP instead of TCP [optional]\n"
    "\n";

void udp_connection()
{
  net::ConnectionType conn_type = net::UDP_ACTIVE;
  net::Connection conn{ "localhost", TTT_DEFAULT_PORT, conn_type };

  LOGERR("Will talk with: %s through %s", conn.getHostname().c_str(),
         conn.isUDP() ? "UDP" : "TCP");

  conn.connect();
  for (std::string line; std::getline(std::cin, line);) {
    conn.write(line);
  }
}

void tcp_connection()
{
  net::ConnectionType conn_type = net::TCP_ACTIVE;
  net::Connection conn{ "localhost", TTT_DEFAULT_PORT, conn_type };

  LOGERR("Will talk with: %s through %s", conn.getHostname().c_str(),
         conn.isUDP() ? "UDP" : "TCP");

  conn.connect();
  for (std::string line; std::getline(std::cin, line);) {
    conn.write(line);
  }
}

void tls_connection()
{
  net::TLSConnection::initialize_TLS();
  net::TLSConnection conn{ "localhost", TTT_DEFAULT_PORT, net::TLS_ACTIVE };

  conn.connect();
  LOGERR("Connection Established!");

  for (std::string line; std::getline(std::cin, line);) {
    conn.write(line);
  }
}

int main(int argc, char* argv[])
{
  /* tcp_connection(); */
  /* udp_connection(); */
  tls_connection();

  return 0;
}
