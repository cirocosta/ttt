#include "ttt/connection.hh"

namespace ttt
{
namespace net
{

Connection::Connection(std::string hname, uint16_t port, ConnectionType type)
    : m_hostname(hname), m_port(port), m_addrinfo(nullptr), m_sockfd(-1)
{
  switch (type) {
    case TTT_CONNECTION_TCP_ACTIVE:
    case TTT_CONNECTION_TCP_PASSIVE:
      m_socktype = ::SOCK_STREAM;
      break;
    case TTT_CONNECTION_UDP_ACTIVE:
    case TTT_CONNECTION_UDP_PASSIVE:
      m_socktype = ::SOCK_DGRAM;
      break;
  }
}

/* void Connection::connect() */
/* { */
/* } */
void Connection::accept()
{
}

void Connection::listen()
{
  net::getaddr(m_hostname, m_socktype);
}

struct addrinfo* Connection::_getAddr(const std::string& host) const
{
  int n;
  struct addrinfo hints;
  struct addrinfo* res;

  memset(&hints, 0, sizeof(struct addrinfo));
  if (m_type == TTT_CONNECTION_TCP_PASSIVE || m_type == TTT_CONNECTION_UDP_PASSIVE)
    hints.ai_flags = AI_PASSIVE;
  else
    hints.ai_flags = AI_CANONNAME;
  hints.ai_family = AF_INET;
  hints.ai_socktype = m_socktype;

  if ((n = getaddrinfo(host.c_str(), NULL, &hints, &res))) {
    LOGERR("getaddr error for %s, %s", host.c_str(), gai_strerror(n));
    return nullptr;
  }

  return res;
}

Connection::~Connection()
{
  net::Close(m_sockfd);
  freeaddrinfo(m_addrinfo);
}
}
};
