#ifndef TTT__USER_HH
#define TTT__USER_HH 

#include <string>
#include <memory>

namespace ttt {

struct User;

typedef std::shared_ptr<User> UserPtr;

struct User
{
  std::string login;
  std::string pwd;

  unsigned score;
  unsigned uptime;
  unsigned id; 

  User (const std::string& l, const std::string& pwd);
};

}; // !ns ttt

#endif
