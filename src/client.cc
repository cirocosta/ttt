#include "ttt/client.hh"
#include "ttt/protocol/parser.hh"

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
  conn->write(Message::str(cmd,args));
}

std::vector<Message> Client::waitMsgs()
{
  conn->read();
  LOGERR("buf: `%s`", conn->getBuffer());
  std::vector<Message> msgs = Parser::parse_msgs(std::string(conn->getBuffer()));

  return msgs;
}

}; // !ns ttt
