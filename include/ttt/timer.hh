#ifndef TTT__TIMER_HH
#define TTT__TIMER_HH

#include "ttt/common.hh"
#include <sys/timerfd.h>
#include <fcntl.h>

#define MILLION 1000000
#define BILLION 1000000000

namespace ttt
{
namespace utils
{

/**
 * Schedules a timer to tick on a fd every 'interval' seconds.
 */
static int schedule_repeating(unsigned interval)
{
  int fd;
  long long nanosecs = interval * BILLION;
  struct itimerspec ts;

  ts.it_value.tv_sec = nanosecs / BILLION;
  ts.it_value.tv_nsec = nanosecs % BILLION;
  ts.it_interval.tv_sec = ts.it_value.tv_sec;
  ts.it_interval.tv_nsec = ts.it_value.tv_nsec;

  PASSERT(~(fd = timerfd_create(CLOCK_MONOTONIC, 0)),
          "timerfd_create: Couldn't create timer:");
  PASSERT(~timerfd_settime(fd, 0, &ts, 0),
          "timerfd_settime: Couldn't activate timer:");

  return fd;
}
}
}

#endif
