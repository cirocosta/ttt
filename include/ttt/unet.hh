#ifndef TTT__UNET_HH
#define TTT__UNET_HH

/**
 * Unix networking wrappers
 */

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

typedef struct sockaddr SA;

static inline int Socket(int family, int type, int protocol)
{
  int n;
  PASSERT(~(n = socket(family, type, protocol)), "socket error:");
  return n;
}

static inline int Bind(int sockfd, const struct sockaddr *addr,
                       socklen_t addrlen)
{
  int n;
  PASSERT(~(n = bind(sockfd, addr, addrlen)), "bind error:");
  return n;
}

static inline int Listen(int sockfd, int backlog)
{
  int n;
  PASSERT(~(n = listen(sockfd, backlog)), "listen error:");
  return n;
}

static inline int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
  int n;

  *addrlen = sizeof(*addr);
  
  // we could have an ECONNABORTED here, in which case we'd not
  // really exit w/ a failure, but ignore.
  PASSERT(~(n = accept(sockfd, addr, addrlen)), "accept error:");

  return n;
}

static inline int Close(int fd)
{
  int n;
  PASSERT(~(n = close(fd)), "close error:");
  return n;
}
}
};

#endif
