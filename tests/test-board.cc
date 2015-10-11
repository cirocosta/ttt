#include "ttt/board.hh"
#include "gtest/gtest.h"

using namespace ttt;
using namespace ttt::game;

TEST(Board, InitPaintOnBuffer)
{
  const char* expected = "000\n000\n000\n";
  char buf[32] = { 0 };
  Board board;

  board.paintOnBuffer(buf);

  ASSERT_EQ(std::string(expected), std::string(buf));
}

TEST(Board, Mark)
{
  {
    const char* expected = "X00\n0X0\n00X\n";
    char buf[32] = { 0 };
    Board board;

    board.mark(MARK_X, 0, 0);
    board.mark(MARK_X, 1, 1);
    board.mark(MARK_X, 2, 2);

    board.paintOnBuffer(buf);
    ASSERT_EQ(std::string(expected), std::string(buf));
    ASSERT_TRUE(board.isMarked(MARK_X, 0, 0));
    ASSERT_FALSE(board.isMarked(MARK_O, 0, 0));
  }

  {
    const char* expected = "O00\n0O0\n00O\n";
    char buf[32] = { 0 };
    Board board;

    board.mark(MARK_O, 0, 0);
    board.mark(MARK_O, 1, 1);
    board.mark(MARK_O, 2, 2);

    board.paintOnBuffer(buf);
    ASSERT_EQ(std::string(expected), std::string(buf));
    ASSERT_FALSE(board.isMarked(MARK_X, 0, 0));
    ASSERT_TRUE(board.isMarked(MARK_O, 0, 0));
  }
}
