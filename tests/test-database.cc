#include "ttt/database.hh"
#include "ttt/common.hh"
#include "gtest/gtest.h"

#include <unistd.h>

using namespace ttt;

static const char* DB_NAME = "/tmp/ttt_test_db";

void fdelete(const char* fname) { PASSERT(!unlink(fname), "fdelete:"); }

int fexists(const char* fname)
{
  if (!access(fname, F_OK))
    return 1;
  return 0;
}

TEST(Database, Creation_inMemory)
{
  Database db;

  ASSERT_EQ(db.mapping.size(), 0);
}

TEST(Database, Creation_persistent)
{
  // remove previously created db's
  if (fexists(DB_NAME))
    fdelete(DB_NAME);

  ASSERT_EQ(0, fexists(DB_NAME));

  // create persistent db
  Database db(DB_NAME);

  ASSERT_EQ(db.mapping.size(), 0);
  ASSERT_EQ(1, fexists(db.fname.c_str()));
}

TEST(Database, InMemory_OperationPut)
{
  Database db;

  ASSERT_EQ(db.mapping.end(), db.mapping.find("ciro"));

  db.put(UserPtr(new User("ciro", "123")));
  auto user = db.mapping.find("ciro");

  ASSERT_NE(db.mapping.end(), user);
  ASSERT_EQ("ciro", user->second->login);
}

TEST(Database, InMemory_OperatonGet)
{
  Database db;

  UserPtr user = db.get("user");
  ASSERT_EQ(nullptr, user);

  db.put(UserPtr(new User("ciro", "123")));
  UserPtr ciro = db.get("ciro");

  ASSERT_EQ("ciro", ciro->login);
  ASSERT_EQ("123", ciro->pwd);
}

// TODO maybe we should return a copy and have the
//      database representing the real database representation?
TEST(Database, InMemory_Modification)
{
  Database db;

  db.put(UserPtr(new User("ciro", "123")));
  auto ciro = db.get("ciro");

  ASSERT_EQ(0, ciro->score);

  ciro->score = 100;

  ASSERT_EQ(100, db.get("ciro")->score);
}
