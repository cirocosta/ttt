#include "ttt/connection.hh"

namespace ttt
{
namespace net
{

Connection::Connection() {}

Connection::Connection(std::string hname, uint16_t port, ConnectionType type)
    : m_hostname(hname), m_port(port)
{
  switch (type) {
    case TTT_CONNECTION_TCP_ACTIVE:
      m_passive = false;
      m_socktype = ::SOCK_STREAM;
      break;
    case TTT_CONNECTION_TCP_PASSIVE:
      m_passive = true;
      m_socktype = ::SOCK_STREAM;
      break;
    case TTT_CONNECTION_UDP_ACTIVE:
      m_passive = false;
      m_socktype = ::SOCK_DGRAM;
      break;
    case TTT_CONNECTION_UDP_PASSIVE:
      m_passive = true;
      m_socktype = ::SOCK_DGRAM;
      break;
  }
}

void Connection::connect()
{
  struct addrinfo* original_addr = _getAddr(m_hostname, m_port);
  struct addrinfo* addr = original_addr;
  int fd;

  ASSERT(original_addr, "Address %s:%u not found", m_hostname.c_str(), m_port);

  do {
    fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

    if (!~fd)
      continue;
    if (!::connect(fd, addr->ai_addr, addr->ai_addrlen))
      break;

    net::Close(fd);
  } while ((addr = addr->ai_next));

  // TODO do not fail on this, but return an 
  //      error or something to recover from
  ASSERT(addr, "No valid address found.");

  setAddrinfo(original_addr, addr);
  setSocket(fd);
}

void Connection::listen()
{
  struct addrinfo* addr = _getAddr(m_hostname, m_port);
  struct addrinfo* original_addr = addr;
  char buf[NAME_MAX] = { 0 };
  const int on = 1;
  int fd;

  if (!m_passive)
    throw std::logic_error("Listen must be used w/ a passive socket.");

  do {
    fd = net::Socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (fd < 0)
      continue;

    ASSERT(~setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)),
           "couldn't set socket options properly");

    if (!bind(fd, addr->ai_addr, addr->ai_addrlen))
      break;

    net::Close(fd);

    // FIXME possible memleak here.
  } while ((addr = addr->ai_next) != NULL);

  ASSERT(addr, "No valid address found.");
  net::Listen(fd, TTT_MAX_BACKLOG);

  inet_ntop(AF_INET, addr->ai_addr, buf, NAME_MAX);
  setAddrinfo(original_addr, addr);
  setSocket(fd);
  setHostname(std::string(buf));
}

ConnectionPtr Connection::accept() const
{
  struct sockaddr_in new_addr;
  socklen_t len;
  char buf[NAME_MAX] = { 0 };
  ConnectionPtr conn = ConnectionPtr(new Connection());

  conn->setSocket(net::Accept(m_sockfd, (SA*)&new_addr, &len));

  inet_ntop(AF_INET, &new_addr.sin_addr, buf, NAME_MAX);
  conn->setPort(ntohs(new_addr.sin_port));
  conn->setHostname(std::string(buf));

  return conn;
}

struct addrinfo* Connection::_getAddr(const std::string& host,
                                      uint16_t port) const
{
  int n;
  struct addrinfo hints;
  struct addrinfo* res;
  const int on = 1;
  const char* node;

  if (host.empty())
    node = NULL;
  else
    node = host.c_str();

  memset(&hints, 0, sizeof(struct addrinfo));
  if (m_passive) {
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
  } else {
    hints.ai_flags = AI_CANONNAME | AI_NUMERICSERV;
  }
  hints.ai_family = AF_INET;
  hints.ai_socktype = m_socktype;

  if ((n = getaddrinfo(node, std::to_string(m_port).c_str(), &hints, &res))) {
    LOGERR("getaddr error for %s, %s", node, gai_strerror(n));
    return nullptr;
  }

  return res;
}

Connection::~Connection() { net::Close(m_sockfd); }
}
};
