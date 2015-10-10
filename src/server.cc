#include "ttt/server.hh"

namespace ttt
{

Server::Server() {}

Server::~Server() { delete m_tcp_conn_events; }

void Server::initUdpListener()
{
  net::Connection conn{ "localhost", TTT_DEFAULT_PORT, net::UDP_PASSIVE };
  net::ConnectionPtr client_conn;
  char buf[1024] = { 0 };
  int n;
  socklen_t len;
  struct sockaddr cliaddr;

  conn.listen();
  while (1) {
    n = recvfrom(conn.getSocket(), buf, 1024, 0, &cliaddr, &len);
    LOGERR("SERVER\t received: %s", buf);
    sendto(conn.getSocket(), buf, 1024, 0, &cliaddr, len);
  }
}

void Server::initTcpListener()
{
  int efd;
  struct epoll_event event;

  net::Connection conn{ "localhost", TTT_DEFAULT_PORT, net::TCP_PASSIVE };
  net::ConnectionPtr client_conn;

  conn.listen();
  conn.makeNonBlocking();

  PASSERT(~(efd = epoll_create1(0)), "epoll_create1:");

  event.data.fd = conn.getSocket();
  event.events = EPOLLIN | EPOLLET;

  PASSERT(~(epoll_ctl(efd, EPOLL_CTL_ADD, conn.getSocket(), &event)),
          "epoll_ctl");
  m_tcp_conn_events =
      (struct epoll_event *)calloc(TTT_MAX_EPOLL_EVENTS, sizeof event);

  std::cout << "Server at " << conn.getHostname() << ":" << conn.getPort()
            << " waiting for clients" << std::endl;

  while (1) {
    int n, i;

    n = epoll_wait(efd, m_tcp_conn_events, TTT_MAX_EPOLL_EVENTS, -1);
    for (i = 0; i < n; i++) {
      // error
      if ((m_tcp_conn_events[i].events & EPOLLERR) ||
          (m_tcp_conn_events[i].events & EPOLLHUP) ||
          (!(m_tcp_conn_events[i].events & EPOLLIN))) {
        LOGERR("epoll error");
        /* net::Close(events[i].data.fd); */
        /* continue; */
      }

      // incomming
      else if (m_tcp_conn_events[i].data.fd == conn.getSocket()) {
        LOGERR("new connection!");
        /* // FIXME if set to nonblocking, inside the accept method */
        //       we should also check if errno == EAGAIN or EWOULDBLOCK
        client_conn = conn.accept();
        client_conn->makeNonBlocking();
        event.data.fd = client_conn->getSocket();
        event.events = EPOLLIN | EPOLLET;

        PASSERT(
            ~(epoll_ctl(efd, EPOLL_CTL_ADD, client_conn->getSocket(), &event)),
            "epoll_ctl");
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
