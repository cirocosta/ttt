#ifndef TTT__USER_HH
#define TTT__USER_HH 

#include <string>
#include <memory>

#include "ttt/connection.hh"

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

  bool active;

  User (const std::string& l, const std::string& pwd);
};

}; // !ns ttt

#endif
