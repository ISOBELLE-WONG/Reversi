#include "textdisplay.h"
#include "cell.h"

TextDisplay::TextDisplay(int n):
    gridSize{n} { 
        theDisplay.resize(n, std::vector<char>(n, '-')); 
    }

void TextDisplay::notify(Subject<Info, State>& whoNotified) {
    auto currInfo = whoNotified.getInfo();
    if (currInfo.colour == Colour::White) {
        theDisplay[currInfo.row][currInfo.col] = 'W';
    } else if (currInfo.colour == Colour::Black) {
        theDisplay[currInfo.row][currInfo.col] = 'B';
    } else {
        theDisplay[currInfo.row][currInfo.col] = '-';
    }
}

std::ostream& operator<<(std::ostream& out, const TextDisplay& td) {
    for (auto r: td.theDisplay) {
        for (auto c: r) {
            out << c;
        }
        out << std::endl;
    }
    return out;
}
