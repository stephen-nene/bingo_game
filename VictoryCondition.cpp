
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
// STUB
  return false;
}

VerticalLine::VerticalLine()
  : VictoryCondition{"Daub five squares in a vertical line.",
                     BingoTypes::VERTICAL_LINE} {}

VerticalLine::~VerticalLine() {}

bool VerticalLine::hasWon(std::vector<Square*> grid) {
// STUB
  return false;
}

AnyLine::AnyLine()
  : VictoryCondition
    {"Daub five squares in a horizontal, vertical or diagonal line.",
     BingoTypes::ANY_LINE} {}

AnyLine::~AnyLine() {}

bool AnyLine::hasWon(std::vector<Square*> grid) {
// STUB
  return false;
}


Blackout::Blackout()
  : VictoryCondition{"Daub all squares on the card.", BingoTypes::BLACKOUT} {}

Blackout::~Blackout() {}

bool Blackout::hasWon(std::vector<Square*> grid) {
  return checkSquareSet(grid);
}
