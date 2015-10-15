#ifndef TTT__COMMON_H
#define TTT__COMMON_H

#include "ttt/constants.hh"

#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <errno.h>

#define LOG(__msg, ...)                                                        \
  do {                                                                         \
    fprintf(stdout, "\n(LOG)\t");                                              \
    fprintf(stdout, __msg, ##__VA_ARGS__);                                     \
  } while (0)

#define LOGERR(__msg, ...)                                                     \
  do {                                                                         \
    fprintf(stderr, "\n(LOGERR)\t");                                           \
    fprintf(stderr, __msg, ##__VA_ARGS__);                                     \
  } while (0)

#define ASSERT(__cond, __msg, ...)                                             \
  do {                                                                         \
    if (!(__cond)) {                                                           \
      fprintf(stderr, "\n" __BASE_FILE__ ": %2d\n", __LINE__);                 \
      fprintf(stderr, "Assertion `%s` failed\n", #__cond);                     \
      fprintf(stderr, "\t" __msg "\n", ##__VA_ARGS__);                         \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

#define PASSERT(__cond, __msg, ...)                                            \
  do {                                                                         \
    if (!(__cond)) {                                                           \
      fprintf(stderr, "\n" __BASE_FILE__ ": %2d\n", __LINE__);                 \
      fprintf(stderr, __msg, ##__VA_ARGS__);                                   \
      fprintf(stderr, "%s\n", strerror(errno));                                \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

namespace ttt
{
namespace utils
{

static unsigned long to_ul(const std::string& str)
{
  char* end;
  unsigned long ul = std::strtoul(str.c_str(), &end, 10);

  PASSERT(str.c_str() != end, "Can't convert string `%s` to unsigned.",
          str.c_str());

  return ul;
}

} // !ns utils
} // !ns ttt

#ifndef NDEBUG
#define DASSERT(__cond, __msg, ...) ASSERT(__cond, __msg, #__VA_ARGS__)
#else
#define DASSERT(__cond, __msg, ...)                                            \
  do {                                                                         \
  } while (0)
#endif

#endif // ! TTT__COMMON_H
