#ifndef TTT__CONNECTION_HH
#define TTT__CONNECTION_HH

#include "ttt/unet.hh"

#include <arpa/inet.h>
#include <linux/limits.h>
#include <iostream>
#include <memory>
#include <string>

namespace ttt
{
namespace net
{

struct AddressInfo {
  struct addrinfo* start;
  struct addrinfo* curr;

  AddressInfo() : curr(NULL), start(NULL) {}

  ~AddressInfo()
  {
    if (start)
      freeaddrinfo(start);
  }
};

typedef enum ConnectionType {
  TTT_CONNECTION_UDP_ACTIVE,
  TTT_CONNECTION_UDP_PASSIVE,
  TTT_CONNECTION_TCP_ACTIVE,
  TTT_CONNECTION_TCP_PASSIVE,
} ConnectionType;

class Connection;
typedef std::unique_ptr<Connection> ConnectionPtr;

class Connection
{
private:
  std::string m_hostname;
  uint16_t m_port;
  bool m_passive;
  int m_socktype;
  int m_sockfd;
  AddressInfo m_addrinfo;

protected:
  Connection();

public:
  Connection(std::string hostname, uint16_t port, ConnectionType t);
  ~Connection();

  void connect();
  ConnectionPtr accept() const;
  void listen();

  inline const std::string getHostname() { return m_hostname; }
  inline const uint16_t getPort() { return m_port; }

  inline void setAddrinfo(struct addrinfo* start, struct addrinfo* curr)
  {
    m_addrinfo.start = start;
    m_addrinfo.curr = curr;
  }
  inline void setSocket(int socket) { m_sockfd = socket; }
  inline void setPort(uint16_t port) { m_port = port; }
  inline void setHostname(const std::string& buf) { m_hostname = buf; }

private:
  struct addrinfo* _getAddr(const std::string& host, uint16_t port) const;
};
}
}

#endif
