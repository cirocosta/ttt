#include "ttt/user.hh"

namespace ttt {

  User::User (const std::string& l, const std::string& p)
    : login(l), pwd(p), score(0)
  {
  }
  
};
