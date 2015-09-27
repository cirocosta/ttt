#ifndef TTT__CONNECTION_HH
#define TTT__CONNECTION_HH

#include "ttt/unet.hh"
#include "ttt/constants.hh"

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
  ConnectionType m_type;

protected:
  char p_buf[TTT_MAX_BUFSIZE];
  Connection();

public:
  Connection(const std::string& hostname, uint16_t port, ConnectionType t);
  ~Connection();

  ssize_t write(const std::string& content) const;
  ssize_t read();

  void connect();               //  client
  ConnectionPtr accept() const; //  server
  void listen();                //  server

  inline bool isUDP() { return m_socktype == SOCK_DGRAM; }
  inline bool isPassive() { return m_passive; }

  inline const std::string getHostname() const { return m_hostname; }
  inline const uint16_t getPort() const { return m_port; }
  inline const int getSocket() const { return m_sockfd; }
  inline const char* getBuffer() const { return p_buf; }
  inline const ConnectionType getType() const { return m_type; }

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
