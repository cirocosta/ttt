#ifndef TTT__ROOM_HH
#define TTT__ROOM_HH

#include "ttt/board.hh"
#include "ttt/user.hh"

#include <memory>
using std::shared_ptr;

namespace ttt
{

struct Room {
  unsigned id;
  game::Board board;
  std::shared_ptr<User> user_a;
  std::shared_ptr<User> user_b;

  Room(shared_ptr<User> ua, shared_ptr<User> ub);

  void start();
  void end();  
};

}; // !ns ttt

#endif
