#include "ttt/connection.hh"
#include "ttt/tls_connection.hh"

#include <sys/epoll.h>
#include <iostream>

#define MAXEVENTS 10

using namespace ttt;

void udp_connection()
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

void tcp_connection()
{
  int efd;
  struct epoll_event event;
  struct epoll_event *events;

  net::Connection conn{ "localhost", TTT_DEFAULT_PORT, net::TCP_PASSIVE };
  net::ConnectionPtr client_conn;

  conn.listen();
  conn.makeNonBlocking();

  PASSERT(~(efd = epoll_create1(0)), "epoll_create1:");

  event.data.fd = conn.getSocket();
  event.events = EPOLLIN | EPOLLET;

  PASSERT(~(epoll_ctl(efd, EPOLL_CTL_ADD, conn.getSocket(), &event)),
          "epoll_ctl");
  events = (struct epoll_event *)calloc(MAXEVENTS, sizeof event);

  std::cout << "Server at " << conn.getHostname() << ":" << conn.getPort()
            << " waiting for clients" << std::endl;

  while (1) {
    int n, i;

    n = epoll_wait(efd, events, MAXEVENTS, -1);
    for (i = 0; i < n; i++) {
      // error
      if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) ||
          (!(events[i].events & EPOLLIN))) {
        LOGERR("epoll error");
        /* net::Close(events[i].data.fd); */
        /* continue; */
      }

      // incomming
      else if (events[i].data.fd == conn.getSocket()) {
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
  /* client_conn = conn.accept(); */
  /* std::cout << "client Connected: " << client_conn->getHostname() << std::endl; */

  /* while (client_conn->read() > 0) { */
  /*   LOGERR("Server received: %s", client_conn->getBuffer()); */
  /* } */
}

void tls_connection()
{
  net::TLSConnection conn{ "localhost", TTT_DEFAULT_PORT, net::TLS_PASSIVE };
  net::TLSConnectionPtr client_conn;

  conn.getConnection()->listen();

  // TODO provide a display
  LOGERR("Server at %s:%u waiting for clients",
         conn.getConnection()->getHostname().c_str(),
         conn.getConnection()->getPort());

  client_conn = conn.accept();
  LOGERR("Client connected: %s",
         client_conn->getConnection()->getHostname().c_str());

  while (client_conn->read() > 0) {
    LOGERR("Server received: %s", client_conn->getConnection()->getBuffer());
  }
}

// TEST ONLY
int main(int argc, char *argv[])
{
  tcp_connection();
  /* udp_connection(); */
  /* tls_connection(); */

  return 0;
}
