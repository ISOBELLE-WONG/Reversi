#include "cell.h"
#include <iostream>

Direction Cell::getDirection(Subject<Info, State>& whoFrom) {
    Info subInfo = whoFrom.getInfo();
    Info currInfo = this->getInfo();
    Direction computedDirection = Direction::N;

    if (subInfo.row > currInfo.row && subInfo.col > currInfo.col) {
        computedDirection = Direction::SE;
    } else if (subInfo.row > currInfo.row && subInfo.col == currInfo.col) {
        computedDirection = Direction::S;
    } else if (subInfo.row > currInfo.row && subInfo.col < currInfo.col) {
        computedDirection = Direction::SW;
    } else if (subInfo.row == currInfo.row && subInfo.col > currInfo.col) {
        computedDirection = Direction::E;
    } else if (subInfo.row == currInfo.row && subInfo.col < currInfo.col) {
        computedDirection = Direction::W;
    } else if (subInfo.row < currInfo.row && subInfo.col > currInfo.col) {
        computedDirection = Direction::NE;
    } else if (subInfo.row < currInfo.row && subInfo.col == currInfo.col) {
        computedDirection = Direction::N;
    } else if (subInfo.row < currInfo.row && subInfo.col < currInfo.col) {
        computedDirection = Direction::NW;
    }
    return computedDirection;
}

Direction Cell::reverseDirection(Direction d) {
    Direction computedDirection = Direction::N;
    if (d == Direction::E) {
        computedDirection = Direction::W;
    } else if (d == Direction::W) {
        computedDirection = Direction::E;
    } else if (d == Direction::N) {
        computedDirection = Direction::S;
    } else if (d == Direction::S) {
        computedDirection = Direction::N;
    } else if (d == Direction::NW) {
        computedDirection = Direction::SE;
    } else if (d == Direction::NE) {
        computedDirection = Direction::SW;
    } else if (d == Direction::SW) {
        computedDirection = Direction::NE;
    } else if (d == Direction::SE) {
        computedDirection = Direction::NW;
    }
    return computedDirection;
}

Cell::Cell(size_t row, size_t col): 
    r{row}, c{col}, colour{Colour::NoColour} { }

void Cell::setPiece(Colour colour) {
    State s{StateType::NewPiece, colour, Direction::N};
    this->colour = colour;
    this->setState(s);
}

void Cell::toggle() {
    if (colour == Colour::Black) {
        colour = Colour::White;
    } else if (colour == Colour::White) {
        colour = Colour::Black;
    }
}

void Cell::notify(Subject<Info, State>& whoFrom) {
    State subState = whoFrom.getState();
    State currState = this->getState();
    Info subInfo = whoFrom.getInfo();
    Info currInfo = this->getInfo();
    // if this observer is not coloured
    if (currInfo.colour == Colour::NoColour) {
        return;
    }
    // subject: NewPiece
    if (subState.type == StateType::NewPiece) {
        if (subInfo.colour != currInfo.colour) {
            //std::cout << "object and subject diff colour" << std::endl;
            Direction mySDirection = getDirection(whoFrom);
            State currState{StateType::Relay, currInfo.colour, mySDirection};            
            this->setState(currState);
            this->notifyObservers();
        }
    // subject: Reply
    } else if (subState.type == StateType::Reply) {
        Direction mySDirection = getDirection(whoFrom);
        if (currState.type == StateType::Relay) {
            if (mySDirection == subState.direction) {
                if (currInfo.colour != subInfo.colour) {
                    this->toggle();
                    State currState{StateType::Reply, subInfo.colour, subState.direction};
                    this->setState(currState);
                    this->notifyObservers();
                }

            }
        }
    // subject: Relay
    } else if (subState.type == StateType::Relay) {
        Direction mySDirection = getDirection(whoFrom);
        if (mySDirection == subState.direction) {
            if(subInfo.colour == currInfo.colour) {
                State currState{StateType::Relay, currInfo.colour, mySDirection};              
                this->setState(currState);
                this->notifyObservers();
            } else if (subInfo.colour != currInfo.colour) {
                Direction currDirection = reverseDirection(mySDirection);
                State currState{StateType::Reply, currInfo.colour, currDirection};
                this->setState(currState);
                this->notifyObservers();
            }
        }
    }  
}

Info Cell::getInfo() const {
    return Info{r, c, colour};
}
