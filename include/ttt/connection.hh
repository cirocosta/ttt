#ifndef TTT__CONNECTION_HH
#define TTT__CONNECTION_HH

namespace ttt
{
namespace net
{

class Connection
{
public:
  struct addrinfo* addrinfo;
  int sockfd;
  std::string hostname;
  uint16_t port;
};
}
}


#endif
