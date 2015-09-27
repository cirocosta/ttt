#ifndef TTT__SERVER_HH
#define TTT__SERVER_HH

#include "ttt/connection.hh"
#include "ttt/tls_connection.hh"

namespace ttt
{

using namespace net;

class Server
{
private:
  // server will handle all 3 kinds of 
  // connections.
  ConnectionPtr m_server_connection;
public:
  Server();
  ~Server();
};
};

#endif