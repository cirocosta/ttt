#include "ttt/server.hh"

namespace ttt
{

using namespace net;
using namespace protocol;

Server::Server()
{
  m_tcp_conn = ConnectionPtr(
      new Connection{ "localhost", TTT_DEFAULT_PORT, TCP_PASSIVE });
  m_udp_conn = ConnectionPtr(
      new Connection{ "localhost", TTT_DEFAULT_PORT, UDP_PASSIVE });
}

Server::~Server() {}

#if 0
void Server::initUdpListener()
{
  Connection conn{ "localhost", TTT_DEFAULT_PORT, UDP_PASSIVE };
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

  ConnectionPtr conn;
  Connection* tmp_conn;

  m_udp_conn->listen();
  m_udp_conn->makeNonBlocking();
  m_tcp_conn->listen();
  m_tcp_conn->makeNonBlocking();

  epoll.add(m_tcp_conn->getSocket(), EPOLLIN | EPOLLET);
  epoll.add(m_udp_conn->getSocket(), EPOLLIN | EPOLLET);

  m_connections[m_udp_conn->getSocket()] = std::move(m_udp_conn);

  std::cout << "Server at " << m_tcp_conn->getHostname() << ":"
            << m_tcp_conn->getPort() << " waiting for clients" << std::endl;

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
      else if (epoll.events[i].data.fd == m_tcp_conn->getSocket()) {
        LOGERR("new connection!");

        // FIXME if set to nonblocking, inside the accept method
        //       we should also check if errno == EAGAIN or EWOULDBLOCK
        conn = m_tcp_conn->accept();
        conn->makeNonBlocking();

        epoll.add(conn->getSocket(), EPOLLIN | EPOLLET);
        m_connections[conn->getSocket()] = std::move(conn);

        continue;
      }

      // data on a fd waiting to be read
      else {
        tmp_conn = m_connections[epoll.events[i].data.fd].get();
        tmp_conn->read();
        respond(tmp_conn);
      }
    }
  }
}

void Server::respond(Connection* conn)
{
  Message msg = Parser::parse_msg(std::string(conn->getBuffer()));

  if (msg.command != CMD_IN) {
    return;
  }

  conn->write(Message::str(RPL_OK, { "Welcome!" }));
}
};
