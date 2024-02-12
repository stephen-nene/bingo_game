
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
    for (int row = 0; row < 5; ++row) {
        int daubedCount = 0;
        for (int col = 0; col < 5; ++col) {
            if (grid[row * 5 + col]->getDaubState()->isDaubed()) {
                ++daubedCount;
            }
        }
        if (daubedCount == 5) {
            return true; // Horizontal line found
        }
    }
    return false; // No horizontal line found
}

VerticalLine::VerticalLine()
  : VictoryCondition{"Daub five squares in a vertical line.",
                     BingoTypes::VERTICAL_LINE} {}

VerticalLine::~VerticalLine() {}

bool VerticalLine::hasWon(std::vector<Square*> grid) {
    for (int col = 0; col < 5; ++col) {
        int daubedCount = 0;
        for (int row = 0; row < 5; ++row) {
            if (grid[row * 5 + col]->getDaubState()->isDaubed()) {
                ++daubedCount;
            }
        }
        if (daubedCount == 5) {
            return true; // Vertical line found
        }
    }
    return false; // No vertical line found
}

AnyLine::AnyLine()
  : VictoryCondition
    {"Daub five squares in a horizontal, vertical or diagonal line.",
     BingoTypes::ANY_LINE} {}

AnyLine::~AnyLine() {}

bool AnyLine::hasWon(std::vector<Square*> grid) {
    // Check horizontal lines
    for (int row = 0; row < 5; ++row) {
        int daubedCount = 0;
        for (int col = 0; col < 5; ++col) {
            if (grid[row * 5 + col]->getDaubState()->isDaubed()) {
                ++daubedCount;
            }
        }
        if (daubedCount == 5) {
            return true; // Horizontal line found
        }
    }
    // Check vertical lines
    for (int col = 0; col < 5; ++col) {
        int daubedCount = 0;
        for (int row = 0; row < 5; ++row) {
            if (grid[row * 5 + col]->getDaubState()->isDaubed()) {
                ++daubedCount;
            }
        }
        if (daubedCount == 5) {
            return true; // Vertical line found
        }
    }
    // Check diagonal lines
    int daubedCount1 = 0, daubedCount2 = 0;
    for (int i = 0; i < 5; ++i) {
        if (grid[i * 5 + i]->getDaubState()->isDaubed()) {
            ++daubedCount1;
        }
        if (grid[i * 5 + (4 - i)]->getDaubState()->isDaubed()) {
            ++daubedCount2;
        }
    }
    if (daubedCount1 == 5 || daubedCount2 == 5) {
        return true; // Diagonal line found
    }
    return false; // No line found
}

Blackout::Blackout()
  : VictoryCondition{"Daub all squares on the card.", BingoTypes::BLACKOUT} {}

Blackout::~Blackout() {}

bool Blackout::hasWon(std::vector<Square*> grid) {
  return checkSquareSet(grid);
}
