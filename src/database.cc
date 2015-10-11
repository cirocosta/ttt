#include "ttt/database.hh"

namespace ttt
{
Database::Database(const std::string& name) : fname(name)
{
  PASSERT((file = fopen(name.c_str(), "w+")), "fopen(%s, w+):", name.c_str());
  mapping = std::map<std::string, UserPtr>{};
}

Database::Database() : file(NULL)
{
  mapping = std::map<std::string, UserPtr>{};
}

Database::~Database()
{
  if (file) {
    PASSERT(!(fclose(file)), "fclose");
    file = nullptr;
  }
}

void Database::deleteFile() { PASSERT(!unlink(fname.c_str()), "fdelete:"); }

void Database::put(UserPtr user) { mapping[user->login] = user; }

UserPtr Database::get(const std::string& key){
  if (mapping.find(key) == mapping.end())
    return nullptr;
  return mapping[key];
}

/* void update(const std::string& key, const std::string& value); */

}; // !ns ttt
