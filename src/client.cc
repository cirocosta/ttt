#include "ttt/client.hh"

namespace ttt
{

Client::Client(const std::string& addr, bool isudp)
    : server_addr(addr), isUdp(isudp)
{
  conn = isudp ? ConnectionPtr(new Connection(server_addr, TTT_DEFAULT_PORT,
                                              net::UDP_ACTIVE))
               : ConnectionPtr(new Connection(server_addr, TTT_DEFAULT_PORT,
                                              net::TCP_ACTIVE));
}

void Client::init()
{
  conn->connect();

  for (std::string line; std::getline(std::cin, line);) {
    conn->write(line);
  }
}

}; // !ns ttt
