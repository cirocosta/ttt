#ifndef TTT__SERVER_HH
#define TTT__SERVER_HH

#include "ttt/connection.hh"
#include "ttt/tls_connection.hh"

#include "ttt/EPoll.hh"
#include <map>

/**
 * - table of registered users
 * - room states
 *    - who's playing
 *    - what's the current state of the game
 * - scores
 */

namespace ttt
{

using namespace net;

class Server
{
private:
  ConnectionPtr m_tcp_conn;
  ConnectionPtr m_udp_conn;
  std::map<int, ConnectionPtr> m_tcp_children;

  EPoll epoll;

public:
  Server();
  ~Server();
public:
  void initUdpListener();   // one thread
  void initTcpListener();   // another thread
};
};

#endif
