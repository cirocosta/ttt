#ifndef TTT__UNET_HH
#define TTT__UNET_HH

#include "ttt/common.hh"

#include <string>
#include <unistd.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace ttt
{
namespace net
{

// socktype determines whether we're going with udp or tcp
struct addrinfo *getaddr(const std::string &host, int socktype);

inline int Socket(int family, int type, int protocol)
{
  int n;
  ASSERT(~(n = socket(family, type, protocol)), "socket error: %s",
         strerror(errno));
  return n;
}

inline int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int n;
  ASSERT(~(n = bind(sockfd, addr, addrlen)), "bind error: %s", strerror(errno));
  return n;
}

inline int Listen(int sockfd, int backlog)
{
  int n;
  ASSERT(~(n = listen(sockfd, backlog)), "bind error: %s", strerror(errno));
  return n;
}

inline int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
  int n;

  *addrlen = sizeof(*addr);
  // we could have an ECONNABORTED here, in which case we'd not
  // really exit w/ a failure, but ignore.
  ASSERT(~(n = accept(sockfd, addr, addrlen)), "accept error: %s",
         strerror(errno));

  return n;
}

inline int Close(int fd)
{
  int n;
  ASSERT(~(n = close(fd)), "close error: %s", strerror(errno));
  return n;
}

inline int Write(int fd, const void *buf, size_t count)
{
  int n;
  ASSERT(~(n = write(fd, buf, count)), "write error: %s", strerror(errno));
  return n;
}

inline int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int n;
  ASSERT(~(n = connect(sockfd, addr, addrlen)), "connect error: %s",
         strerror(errno));
  return n;
}
}
};

#endif
