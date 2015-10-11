#include "ttt/board.hh"

namespace ttt
{
namespace game
{

Board::Board() { memset(m_data, '0', TTT_BOARD_SIZE + 1); }
Board::~Board() {}

void Board::paintOnBuffer(char* buf) const
{
  strncpy((char*)buf, m_data, 3);
  buf[3] = '\n';
  strncpy((char*)buf + 4, m_data + 3, 3);
  buf[7] = '\n';
  strncpy((char*)buf + 8, m_data + 6, 3);
  buf[11] = '\n';
  buf[12] = '\0';
}

bool Board::isMarked(BOARD_MARK mark, unsigned row, unsigned column) const
{
  char c = m_data[columnRowToPos(row, column)];

  switch (c) {
    case '0':
      return mark == MARK_EMPTY;
    case 'X':
      return mark == MARK_X;
    case 'O':
      return mark == MARK_O;
  }

  return false;
}

} // !ns game

std::ostream& operator<<(std::ostream& o, const game::Board& board)
{
  char buf[13] = { 0 };

  board.paintOnBuffer(buf);

  o << buf;

  return o;
}
};
