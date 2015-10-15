#ifndef TTT__SERVER_HH
#define TTT__SERVER_HH

#include "ttt/connection.hh"
#include "ttt/tls_connection.hh"
#include "ttt/EPoll.hh"
#include "ttt/protocol/parser.hh"
#include "ttt/user.hh"

#include <map>
#include <vector>

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

  std::map<int, ConnectionPtr> m_connections;
  std::map<long, protocol::Message> m_messages;

  std::vector<UserPtr> m_users;

  EPoll epoll;

public:
  Server();
  ~Server();

  void init();
  void respond(Connection* conn);
};
};

#endif
