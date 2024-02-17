
#include <algorithm>
#include <string>
#include <vector>

#include "VictoryCondition.h"

VictoryCondition::VictoryCondition(std::string description,
                                   BingoTypes::victoryType victory)
  : _description{description}, _victoryType{victory} {}

VictoryCondition::~VictoryCondition() {}

std::string VictoryCondition::getDescription() {
  return _description;
}

BingoTypes::victoryType VictoryCondition::getVictoryType() {
  return _victoryType;
}

bool VictoryCondition::checkSquareSet(std::vector<Square*> squares) {
  for (unsigned i = 0; i < squares.size(); ++i) {
    if (!((squares[i]->getDaubState())->isDaubed()
          && (squares[i]->getDaubState())->isCorrect()))
      return false;
  }
  return true;
}

HorizontalLine::HorizontalLine()
  : VictoryCondition{"Daub five squares in a horizontal line.",
                     BingoTypes::HORIZONTAL_LINE} {}

HorizontalLine::~HorizontalLine() {}

bool HorizontalLine::hasWon(std::vector<Square*> grid) {
    for (unsigned i = 0; i < 5; ++i) {
        unsigned count = 0;
        for (unsigned j = 0; j < 5; ++j) {
            if ((grid[i * 5 + j]->getDaubState())->isDaubed() &&
                (grid[i * 5 + j]->getDaubState())->isCorrect()) {
                ++count;
            }
        }
        if (count == 5) {
            return true;
        }
    }
    return false;
}
VerticalLine::VerticalLine()
  : VictoryCondition{"Daub five squares in a vertical line.",
                     BingoTypes::VERTICAL_LINE} {}

VerticalLine::~VerticalLine() {}

bool VerticalLine::hasWon(std::vector<Square*> grid) {
    for (unsigned i = 0; i < 5; ++i) {
        unsigned count = 0;
        for (unsigned j = 0; j < 5; ++j) {
            if ((grid[j * 5 + i]->getDaubState())->isDaubed() &&
                (grid[j * 5 + i]->getDaubState())->isCorrect()) {
                ++count;
            }
        }
        if (count == 5) {
            return true;
        }
    }
    return false;
}


AnyLine::AnyLine()
  : VictoryCondition
    {"Daub five squares in a horizontal, vertical or diagonal line.",
     BingoTypes::ANY_LINE} {}

AnyLine::~AnyLine() {}

bool AnyLine::hasWon(std::vector<Square*> grid) {
    // Check horizontal lines
    for (unsigned i = 0; i < 5; ++i) {
        unsigned count = 0;
        for (unsigned j = 0; j < 5; ++j) {
            if ((grid[i * 5 + j]->getDaubState())->isDaubed() &&
                (grid[i * 5 + j]->getDaubState())->isCorrect()) {
                ++count;
            }
        }
        if (count == 5) {
            return true;
        }
    }

    // Check vertical lines
    for (unsigned i = 0; i < 5; ++i) {
        unsigned count = 0;
        for (unsigned j = 0; j < 5; ++j) {
            if ((grid[j * 5 + i]->getDaubState())->isDaubed() &&
                (grid[j * 5 + i]->getDaubState())->isCorrect()) {
                ++count;
            }
        }
        if (count == 5) {
            return true;
        }
    }

    // Check main diagonal
    unsigned countMainDiagonal = 0;
    for (unsigned i = 0; i < 5; ++i) {
        if ((grid[i * 5 + i]->getDaubState())->isDaubed() &&
            (grid[i * 5 + i]->getDaubState())->isCorrect()) {
            ++countMainDiagonal;
        }
    }
    if (countMainDiagonal == 5) {
        return true;
    }

    // Check anti-diagonal
    unsigned countAntiDiagonal = 0;
    for (unsigned i = 0; i < 5; ++i) {
        if ((grid[i * 5 + (4 - i)]->getDaubState())->isDaubed() &&
            (grid[i * 5 + (4 - i)]->getDaubState())->isCorrect()) {
            ++countAntiDiagonal;
        }
    }
    if (countAntiDiagonal == 5) {
        return true;
    }

    return false;
}



Blackout::Blackout()
  : VictoryCondition{"Daub all squares on the card.", BingoTypes::BLACKOUT} {}

Blackout::~Blackout() {}

bool Blackout::hasWon(std::vector<Square*> grid) {
  return checkSquareSet(grid);
}
