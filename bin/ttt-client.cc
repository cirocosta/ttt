#include "ttt/connection.hh"

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

void echo_udp(net::Connection& conn)
{
  conn.connect();
  // send something
  for (std::string line; std::getline(std::cin, line);) {
    conn.write(line);
  }
}

void echo_tcp(net::Connection& conn)
{
  conn.connect();

  for (std::string line; std::getline(std::cin, line);) {
    LOGERR("TCP\tJust said: %s", line.c_str());
  }
}

int main(int argc, char* argv[])
{
  if (argc < 2) {
    fprintf(stderr, "%s", CLI_HELP);
    return 1;
  }

  net::ConnectionType conn_type = argc == 3 ? net::TTT_CONNECTION_UDP_ACTIVE
                                            : net::TTT_CONNECTION_TCP_ACTIVE;
  net::Connection conn{ std::string(argv[1]), TTT_DEFAULT_PORT, conn_type };

  LOGERR("argc = %d", argc);
  LOGERR("Will talk with: %s through %s", conn.getHostname().c_str(),
         conn.isUDP() ? "UDP" : "TCP");

  if (conn.isUDP()) {
    echo_udp(conn);
  } else {
    echo_tcp(conn);
  }

  return 0;
}
