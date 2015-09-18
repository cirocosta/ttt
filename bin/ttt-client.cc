#include "ttt/connection.hh"
#include <iostream>

using namespace ttt;

int main(int argc, char *argv[])
{
  net::Connection conn {"localhost", 8080, net::TTT_CONNECTION_TCP_ACTIVE};
  conn.connect();
  
  return 0;
}
