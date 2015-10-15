#ifndef TTT__SERVER_HH
#define TTT__SERVER_HH

#include "ttt/connection.hh"
#include "ttt/tls_connection.hh"
#include "ttt/EPoll.hh"
#include "ttt/protocol/parser.hh"
#include "ttt/user.hh"

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

  std::map<int, ConnectionPtr> m_connections;
  std::map<unsigned, UserPtr> m_users;

  EPoll epoll;
  unsigned m_uid_count;

public:
  Server();
  ~Server();

  inline unsigned generateUID() { return m_uid_count++; }

  void init();
  void respond(Connection* conn);

  void cmd_login(Connection* conn, const std::string&, const std::string&);
};
};

#endif
