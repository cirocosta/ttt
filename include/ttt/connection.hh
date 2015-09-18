#ifndef TTT__CONNECTION_HH
#define TTT__CONNECTION_HH

#include "ttt/unet.hh"
#include <iostream>
#include <string>

namespace ttt
{
namespace net
{

typedef enum ConnectionType {
  TTT_CONNECTION_UDP_ACTIVE,
  TTT_CONNECTION_UDP_PASSIVE,
  TTT_CONNECTION_TCP_ACTIVE,
  TTT_CONNECTION_TCP_PASSIVE,
} ConnectionType;

class Connection
{
private:
  struct addrinfo* m_addrinfo;
  int m_sockfd;
  std::string m_hostname;
  uint16_t m_port;
  ConnectionType m_type;
  int m_socktype;

public:
  Connection(std::string hostname, uint16_t port, ConnectionType t);
  ~Connection();

  void connect(const std::string& host, uint16_t port);
  void accept();
  void listen();

private:
  struct addrinfo* _getAddr(const std::string& host) const;
};
}
}

#endif
