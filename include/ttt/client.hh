#ifndef TTT__CLIENT_HH
#define TTT__CLIENT_HH

#include "ttt/tls_connection.hh"

/**
 * - connect to the server
 *    - TCP
 *    - UDP
 * - chat with another player
 *    - directly (p2p)
 * - exchange info with the server
 *    - login
 *    - heartbeat
 *    - logout
 *    - state exchange
 */

namespace ttt
{

using namespace net;

struct Client {
  // talk with the server
  std::string server_addr;
  bool isUdp;

  ConnectionPtr conn;

  Client(const std::string& saddr, bool isUdp);
  void init();
};

}; // !ns ttt

#endif
