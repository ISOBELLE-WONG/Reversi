#include <iostream>
#include <string>
// You may include other allowed headers, as needed
#include "grid.h"
#include "state.h"
using namespace std;

// Do not remove any code; do not add code other than where indicated.

int main(int argc, char *argv[]) {
  cin.exceptions(ios::eofbit|ios::failbit);
  string cmd;
  Grid g;

  // Add code here
  bool isValidGrid = true;
  Colour next = Colour::Black;

  try {
  while (true) {
    cin >> cmd;
    if (cmd == "new") {
      int n;
      cin >> n;
      // Add code here
      if (n % 2 != 0 || n < 4) {
        isValidGrid = false;
      } else {
        isValidGrid = true;
        g.init(n);
        std::cout << g;
      }
    } else if (cmd == "play") {
      int r = 0, c = 0;
      cin >> r >> c;
      // Add code here
      bool isValidPlay = g.setPiece(r, c, next);
      
      if (isValidPlay && isValidGrid) {
        std::cout << g;
        if (next == Colour::White) {
          next = Colour::Black;
        } else {
          next = Colour::White;
        }
      } else if (isValidPlay) {
        if (next == Colour::White) {
          next = Colour::Black;
        } else {
          next = Colour::White;
        }
      }

      if (g.isFull()) {
        if (isValidGrid) {
          Colour result = g.whoWon();
          if (result == Colour::White) {
            std::cout << "White wins!" << std::endl;
          } else if (result == Colour::Black) {
            std::cout << "Black wins!" << std::endl;
          } else {
            std::cout << "Tie!" << std::endl;
          }
        }
        break;
      }
    }
  }
  }
  catch (ios::failure &) {} 
}
