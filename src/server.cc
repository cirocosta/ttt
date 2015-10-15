#include "ttt/server.hh"

namespace ttt
{

using namespace net;
using namespace protocol;

Server::Server() : m_uid_count(0)
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
  const std::string src (conn->getBuffer());

  if (src.empty()) {
    LOGERR("empty buffer received");
    return;
  }

  Message msg = Parser::parse_msg(std::string(conn->getBuffer()));

  switch (msg.command) {
    case CMD_IN:
      cmd_login(conn, msg.args[0], msg.args[1]);
      break;
    default:
      conn->write(Message::str(RPL_ERR, { "Unrecognized Command" }));
  }
}

void Server::cmd_login(Connection* conn, const std::string& login,
                       const std::string& pwd)
{
  for (const auto user : m_users) {
    if (user.second->login != login)
      continue;
    if (user.second->pwd != pwd) {
      conn->write(Message::str(RPL_ERR, { "Wrong Password" }));
      break;
    }

    conn->write(Message::str(RPL_OK, { "Welcome Back!" }));
    return;
  }

  unsigned new_id = generateUID();
  const auto new_user_pos =
      m_users.emplace(new_id, std::make_shared<User>(login, pwd));

  new_user_pos.first->second->id = new_id;
  conn->write(Message::str(
      RPL_OK, { "Welcome!", std::to_string(new_user_pos.first->second->id) }));
}
};
