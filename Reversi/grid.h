#ifndef GRID_H
#define GRID_H
#include <iostream>
#include <vector>
#include <cstddef>
#include "cell.h"
#include "state.h"
#include "info.h"

class TextDisplay;
template <typename InfoType, typename StateType> class Observer;

class Grid {
  std::vector<std::vector<Cell>> theGrid;  // The actual grid.
  TextDisplay *td = nullptr; // The text display.
  Observer<Info, State> *ob = nullptr;  // Another observer (intent:  graphics)
  // Add private members, if necessary.
  int dimension, numCells, numWhite, numBlack, numColoured = 0;
  int getBlackCount() const;
  bool initialized = false;

 public:
  ~Grid();
  
  void setObserver(Observer<Info, State> *ob);
  bool isFull() const;  // Is the game over, i.e., is the grid full?
  Colour whoWon() const; // Who has more pieces when the board is full?
  void init(size_t n); // Sets up an n x n grid.  Clears old grid, if necessary.
  bool setPiece(size_t r, size_t c, Colour colour);  // Plays piece at row r, col c. True if valid placement/coordinate, False otherwise
  void toggle(size_t r, size_t c);  // Flips piece at row r, col c.

  friend std::ostream &operator<<(std::ostream &out, const Grid &g);
};

#endif
