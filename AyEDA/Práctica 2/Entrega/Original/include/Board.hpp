#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include "Cell.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <utility>
#include <vector>

class Board
{
public:
  // Default constructor
  Board() : filas_(0), columnas_(0), border_("reflective"), v_(0), cell_(NULL) {}
  // Constructor if the size of the Board is known
  Board(Cell *[], int, int, const std::string &, const std::optional<State>);
  // Constructor if the file name is known
  Board(const char *, const std::string &, const std::optional<State>);
  // Destructor
  ~Board() { CleanBoard(); }

  // Getters
  int GetFilas() const { return filas_; }
  int GetColumnas() const { return columnas_; }
  std::string GetBorder() const { return border_; }
  std::optional<State> GetV() const { return v_; }
  const Cell &GetCell(const Position &position) const { return cell_[position.first][position.second]; }
  State GetCellState(const Position &position) const;

  // Setters
  void SetFilas(int filas) { filas_ = filas; }
  void SetColumnas(int columnas) { columnas_ = columnas; }
  void SetBorder(const std::string &border) { border_ = border; }
  void SetV(const std::optional<State> v) { v_ = v; }

  // Other methods
  std::size_t Population() const;
  void NextGeneration();
  void RunGame();

  // Overloading operators
  Cell &operator[](const Position &position) const { return cell_[position.first][position.second]; }
  friend std::ostream &operator<<(std::ostream &, const Board &);

private:
  int filas_;
  int columnas_;
  std::string border_;
  std::optional<State> v_;
  Cell **cell_; // Bidimensional array of cells

  // Methods to get the state of the cell
  const State GetReflectiveState(const Position &position) const;
  const State GetPeriodicState(const Position &position) const;
  const State GetOpenState(const Position &position) const;
  const State GetNoBordeState(const Position &position) const;

  // Methods to load and save the board
  void SaveBoard(const char *filename);
  void CopyBoard(Cell **origen, Cell **&destino, int filas, int columnas);
  char InitGame();
  void CleanBoard();

  // Methods to push cells
  void Push_at(const Position &position);
  void Push_back();
  void Push_front();
  void Push_top();
  void Push_bottom();
};

#endif // __BOARD_HPP__