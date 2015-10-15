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
}

void Client::sendMsg(COMMAND cmd, std::initializer_list<std::string> args)
{
  conn->write(Message{.command = cmd,
                      .args = std::vector<std::string>(args) }.toString());
}

Message Client::waitMsg()
{
  Message message {.command = CMD_IN, .args = std::vector<std::string>()};
  conn->read();
  LOGERR("Just read: %s", conn->getBuffer());

  return message;
}

}; // !ns ttt
