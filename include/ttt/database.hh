#ifndef TTT__DATABASE_HH
#define TTT__DATABASE_HH

#include "ttt/common.hh"
#include "ttt/user.hh"

#include <unistd.h>
#include <cstdio>
#include <string>
#include <map>
#include <memory>

/**
 * Key-value plain-text database.
 * On initialization, puts the entire DB in memory.
 * Only persists when needed, i.e, on a save or update.
 */

namespace ttt
{

struct Database {
  std::string fname;
  FILE* file;
  std::map<std::string, UserPtr> mapping;

  Database(const std::string& fname);
  Database(); // in-memory only
  ~Database();

  void put(const UserPtr value);
  UserPtr get(const std::string& key);
  void deleteFile();
};

}; // !ns ttt

#endif
