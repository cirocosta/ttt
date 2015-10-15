#include "ttt/user.hh"

namespace ttt {

  User::User (const std::string& l, const std::string& p)
    : login(l), pwd(p), score(0), uptime(0), id(0), active(false)
  {
  }
  
};
