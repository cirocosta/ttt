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

void Server::init()
{
  struct epoll_event event;
  uint64_t enqueued_hb;

  ConnectionPtr conn;
  Connection* tmp_conn;

  m_udp_conn->listen();
  m_udp_conn->makeNonBlocking();
  m_tcp_conn->listen();
  m_tcp_conn->makeNonBlocking();
  m_heartbeat_fd = utils::schedule_repeating(2);

  epoll.add(m_tcp_conn->getSocket(), EPOLLIN | EPOLLET);
  epoll.add(m_udp_conn->getSocket(), EPOLLIN | EPOLLET);
  epoll.add(m_heartbeat_fd, EPOLLIN | EPOLLET);

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

      else if (epoll.events[i].data.fd == m_heartbeat_fd) {
        LOGERR("HEARTBEAT");
        ::read(m_heartbeat_fd, &enqueued_hb, 8);
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
  const std::string src(conn->getBuffer());

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
      conn->write(Message::str(RPL_INPWD, { "Wrong Password" }));
      break;
    }

    if (user.second->active) {
      conn->write(Message::str(RPL_ALOGGED, { "User already logged in" }));
      break;
    }

    conn->write(
        Message::str(RPL_WELCOME, { std::to_string(user.second->id),
                                    std::to_string(user.second->score) }));
    return;
  }

  unsigned new_id = generateUID();
  const auto new_user =
      m_users.emplace(new_id, std::make_shared<User>(login, pwd)).first->second;

  new_user->id = new_id;
  conn->write(Message::str(RPL_WELCOME, { std::to_string(new_user->id),
                                          std::to_string(new_user->score) }));
}
};
