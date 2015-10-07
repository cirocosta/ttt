#ifndef TTT__CLIENT_HH
#define TTT__CLIENT_HH 

#include "ttt/connection.hh"
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

namespace ttt {

using namespace ttt::net;

};

#endif
