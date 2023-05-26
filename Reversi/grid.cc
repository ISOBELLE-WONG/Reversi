#include "grid.h"
#include "textdisplay.h"

int Grid::getBlackCount() const {
    int result = 0;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            Info sInfo = theGrid[i][j].getInfo();
            if (sInfo.colour == Colour::Black) {
                result += 1;
            } 
        }
    }
    return result;
}

Grid::~Grid() {
    delete td;
    delete ob;
}

void Grid::setObserver(Observer <Info, State> *ob) {
    this->ob = ob;
}

bool Grid::isFull() const {
    return numColoured == numCells;
}

Colour Grid::whoWon() const {
    int bc = getBlackCount();
    int wc = this->numCells - bc;

    if (bc < wc) {
        return Colour::White;
    } else if (wc < bc) {
        return Colour::Black;
    } else {
        return Colour::NoColour;
    }
}


void Grid::init(size_t n) {
    int d = n;
    this->dimension = d;
    this->numCells = d * d;
    if (initialized == true) {
        theGrid.clear();
        delete td;
    }
    td = new TextDisplay{dimension};
    
    int half = d / 2;
    for (int i = 0; i < dimension; i++) {
        std::vector<Cell> rowOfCells;
        for (int j = 0; j < dimension; j++) {
            rowOfCells.emplace_back(i, j);
            if (i == half - 1 && j == half - 1) {
                rowOfCells.back().setPiece(Colour::Black);
            } else if (i == half - 1 && j == half) {
                rowOfCells.back().setPiece(Colour::White);
            } else if (i == half && j == half - 1) {
                rowOfCells.back().setPiece(Colour::White);
            } else if (i == half && j == half) {
                rowOfCells.back().setPiece(Colour::Black);
            }
            td->notify(rowOfCells.back());
            if (ob != nullptr) {
                ob->notify(rowOfCells.back());
            }
        }
        theGrid.emplace_back(rowOfCells);
    }
    this->numBlack = 2;
    this->numWhite = 2;
    this->numColoured = numBlack + numWhite;
    this->initialized = true;
    
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if ( j > 0) { // N
                theGrid[i][j].attach(&theGrid[i][j - 1]);
            }
            if ( j < dimension - 1) {  // S
                theGrid[i][j].attach(&theGrid[i][j + 1]);
            }
            if ( i > 0) { // W                              
                theGrid[i][j].attach(&theGrid[i - 1][j]);
            }            
            if ( i < dimension - 1) { // E
                theGrid[i][j].attach(&theGrid[i + 1][j]);
            }
            if (i > 0 && j > 0) { // NW
                theGrid[i][j].attach(&theGrid[i - 1][j - 1]);
            } 
            if (i > 0 && j < dimension - 1) { // NE
                theGrid[i][j].attach(&theGrid[i - 1][j + 1]);
            }
            if (i < dimension - 1 && j > 0) { // SW
                theGrid[i][j].attach(&theGrid[i + 1][j - 1]);
            }
            if (i < dimension - 1 && j < dimension - 1) { // SE
                theGrid[i][j].attach(&theGrid[i + 1][j + 1]);
            }
        
            theGrid[i][j].attach(td);
            if (ob != nullptr) theGrid[i][j].attach(ob);
        }
    }
}

bool Grid::setPiece(size_t r, size_t c, Colour colour) {
    int n = this->dimension;
    int ri = r;
    int ci = c;
    
    if (ri < 0 || ci < 0 || ri > n - 1 || ci > n - 1) { 
        return false;
    }
    
    Info inf = theGrid[r][c].getInfo();
    if (inf.colour != Colour::NoColour) {
        return false;
    } else {
        theGrid[r][c].setPiece(colour);
        theGrid[ri][ci].notifyObservers(); 
        this->numColoured += 1;
        return true;
    }
}

void Grid::toggle(size_t r, size_t c) {
    theGrid[r][c].toggle();
}

std::ostream& operator<<(std::ostream& out, const Grid& g) {
    out << *(g.td);
    return out;
}
