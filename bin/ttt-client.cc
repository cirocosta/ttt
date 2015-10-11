#include "ttt/client.hh"

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

#if 0
void tls_connection()
{
  net::TLSConnection conn{ "localhost", TTT_DEFAULT_PORT, net::TLS_ACTIVE };

  conn.connect();
  LOGERR("Connection Established!");

  for (std::string line; std::getline(std::cin, line);) {
    conn.write(line);
  }
}
#endif 

int main(int argc, char* argv[])
{
  bool useUdp = false;

  if (argc < 2) {
    std::cout << CLI_HELP << std::endl
              << "Error: at least <address> must be specified"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string addr = argv[1];

  if (argc == 3 && !strcmp(argv[2], "-u"))
    useUdp = true;

  Client client (addr, useUdp);
  client.init();

  return 0;
}
