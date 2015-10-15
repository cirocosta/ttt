#ifndef TTT__CLIENT_HH
#define TTT__CLIENT_HH

#include "ttt/tls_connection.hh"
#include "ttt/protocol/msg.hh"
#include "ttt/user.hh"

#include <initializer_list>

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
using namespace protocol;

struct Client {
  std::string server_addr;
  bool isUdp;
  UserPtr user;

  ConnectionPtr conn;

  Client(const std::string& saddr, bool isUdp);

  void init();
  void sendMsg(COMMAND cmd, std::initializer_list<std::string> args);
  std::vector<Message> waitMsgs();

  void cmd_login();
};

}; // !ns ttt

#endif
