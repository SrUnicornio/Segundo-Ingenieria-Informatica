#ifndef __CELL_HPP__
#define __CELL_HPP__

#include <iostream>
#include <utility>
#include <vector>

typedef int State; // 0 = muerta, 1 = viva
const int kDead = 0;
const int kAlive = 1;
typedef std::pair<int, int> Position; /**
                                          (i-1,j-1)|(i-1,j)|(i-1,j+1)
                                          ---------------------------
                                          (i,j-1)  | (i,j) | (i,j+1)
                                          ---------------------------
                                          (i+1,j-1)|(i+1,j)|(i+1,j+1)
                                      */

class Board;
class Cell
{
public:
  // Default constructor
  Cell() : position_(std::make_pair(0, 0)), state_(0) {}
  // Constructor
  Cell(const Position &position, const State &state = 0) : position_(position), state_(state) {}
  // Destructor
  ~Cell() {}

  // Getters
  State GetState() const { return state_; }
  Position GetPosition() const { return position_; }

  // Setters
  State SetState(State state) { return state_ = state; }
  Position SetPosition(Position position) { return position_ = position; }

  // Other methods
  int NextState(const Board &Board, const Position &position);
  int CountAlive(const Board &Board, const Position &position);
  void UpdateState() { state_ = SetState(nextstate_); }

  // Overloading operators
  friend std::ostream &operator<<(std::ostream &, const Cell &);

private:
  Position position_;
  State state_;
  State nextstate_;
};

#endif // __CELL_HPP__