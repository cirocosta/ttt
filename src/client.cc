#include "ttt/client.hh"
#include "ttt/protocol/parser.hh"

namespace ttt
{

Client::Client(const std::string& addr, bool isudp)
    : server_addr(addr), isUdp(isudp)
{
  conn = isudp ? ConnectionPtr(new Connection(server_addr, TTT_DEFAULT_PORT,
                                              net::UDP_ACTIVE))
               : ConnectionPtr(new Connection(server_addr, TTT_DEFAULT_PORT,
                                              net::TCP_ACTIVE));
}

void Client::init() { conn->connect(); }

void Client::sendMsg(COMMAND cmd, std::initializer_list<std::string> args)
{
  conn->write(Message::str(cmd, args));
}

std::vector<Message> Client::waitMsgs()
{
  conn->read();
  LOGERR("buf: `%s`", conn->getBuffer());
  std::vector<Message> msgs =
      Parser::parse_msgs(std::string(conn->getBuffer()));

  return msgs;
}

void Client::cmd_login()
{
  std::string login;
  std::string pwd;
  double msg_id = 0;

  std::cout << "Welcome to TTT!\n"
            << "Please enter your userid and password.\n"
            << "If you're not registered a new account will be created\n"
            << "\n";

  while (1) {
    std::cout << "User: ";
    std::cin >> login;
    std::cout << "\nPassword: ";
    std::cin >> pwd;

    sendMsg(CMD_IN, { login, pwd });
    std::vector<Message> msgs = waitMsgs();

    if (msgs[0].command == RPL_WELCOME) {
      user = UserPtr(new User(login, pwd));
      user->id = utils::to_ul(msgs[0].args[0]);
      user->score = utils::to_ul(msgs[0].args[1]);

      return;
    }
  }
}

}; // !ns ttt
