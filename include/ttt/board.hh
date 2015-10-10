#ifndef TTT__BOARD_HH
#define TTT__BOARD_HH

#include "ttt/common.hh"
#include <iostream>

namespace ttt
{

namespace game
{

typedef enum BOARD_MARK { MARK_EMPTY = 0, MARK_X, MARK_O } BOARD_MARK;
static const unsigned BOARD_OFFSET = 3;

/**
 * Display: raster
 * Data encoding: Raster corresponds to
 * least significant bit to most significant.
 */
class Board
{
  char m_data[TTT_BOARD_SIZE + 1];

public:
  Board();
  ~Board();

  bool isMarked(BOARD_MARK mark, unsigned row, unsigned column);
  inline void mark(BOARD_MARK mark, unsigned row, unsigned column)
  {
    m_data[columnRowToPos(row, column)] = (mark == MARK_O ? 'O' : 'X');
  }

  inline char const* getData() const { return m_data; }
  inline void setData(char const* data) { strncpy(m_data, data, 9); }
  void paintOnBuffer(char* buf) const;

private:
  constexpr static unsigned columnRowToPos(unsigned row, unsigned column)
  {
    return BOARD_OFFSET * row + column;
  }
};
}

std::ostream& operator<<(std::ostream& o, const game::Board& board);
};

#endif
