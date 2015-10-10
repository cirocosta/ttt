#ifndef TTT__EPOLL_HH
#define TTT__EPOLL_HH

#include <sys/epoll.h>
#include "ttt/constants.hh"
#include "ttt/common.hh"

struct EPoll {
  struct epoll_event** events;
  int efd;

  EPoll(EPoll const& t) = delete;
  EPoll& operator=(EPoll const& t) = delete;

  EPoll(unsigned size)
  {
    /* events = (struct epoll_event*)calloc(TTT_MAX_EPOLL_EVENTS, sizeof
     * *events); */
    events = new struct epoll_event* [TTT_MAX_EPOLL_EVENTS];
    PASSERT(~(efd = epoll_create1(0)), "epoll_create1:");
  }

  ~EPoll() { delete[] events; }

  inline int wait()
  {
    return epoll_wait(efd, *events, TTT_MAX_EPOLL_EVENTS, -1);
  }

  void add(int fd, uint32_t flags, void* ptr = NULL)
  {
    struct epoll_event ev;

    ev.data.fd = fd;
    ev.events = flags;
    ev.data.ptr = ptr;

    PASSERT(~(epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev)), "epoll_ctl");
  }
};

#endif
