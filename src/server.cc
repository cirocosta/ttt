#include "ttt/server.hh"

namespace ttt
{

Server::Server() {}

Server::~Server() {}

#if 0
void Server::initUdpListener()
{
  net::Connection conn{ "localhost", TTT_DEFAULT_PORT, net::UDP_PASSIVE };
  char buf[1024] = { 0 };
  int n;
  socklen_t len;
  struct sockaddr cliaddr;

  conn.listen();
  /* conn.makeNonBlocking(); */

  while (1) {
    n = recvfrom(conn.getSocket(), buf, 1024, 0, &cliaddr, &len);
    LOGERR("SERVER\t received: %s", buf);
    sendto(conn.getSocket(), buf, 1024, 0, &cliaddr, len);
  }
}
#endif

void Server::init()
{
  struct epoll_event event;

  net::Connection udp{ "localhost", TTT_DEFAULT_PORT, net::UDP_PASSIVE };
  net::Connection tcp{ "localhost", TTT_DEFAULT_PORT, net::TCP_PASSIVE };
  net::ConnectionPtr tcp_client;

  udp.listen();
  udp.makeNonBlocking();

  tcp.listen();
  tcp.makeNonBlocking();

  epoll.add(tcp.getSocket(), EPOLLIN | EPOLLET);
  epoll.add(udp.getSocket(), EPOLLIN | EPOLLET);

  std::cout << "Server at " << tcp.getHostname() << ":" << tcp.getPort()
            << " waiting for clients" << std::endl;

  while (1) {
    int n, i;

    n = epoll.wait();

    for (i = 0; i < n; i++) {
      // error
      if ((epoll.events[i].events & EPOLLERR) ||
          (epoll.events[i].events & EPOLLHUP) ||
          (!(epoll.events[i].events & EPOLLIN))) {
        LOGERR("epoll error");
      }

      // incomming
      else if (epoll.events[i].data.fd == tcp.getSocket()) {
        LOGERR("new connection!");

        // FIXME if set to nonblocking, inside the accept method
        //       we should also check if errno == EAGAIN or EWOULDBLOCK
        tcp_client = tcp.accept();
        tcp_client->makeNonBlocking();

        epoll.add(tcp_client->getSocket(), EPOLLIN | EPOLLET);
        continue;
      }

      // data on a fd waiting to be read
      else {
        LOGERR("data to read!");
      }
    }
  }
}
};
