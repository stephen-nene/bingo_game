#include <algorithm>
#include <string>
#include <vector>

#include "BingoTypes.h"
#include "BingoCard.h"
#include "UserInput.h"
#include "VictoryCondition.h"
#include "Square.h"
#include "Exceptions.h"

BingoCard::BingoCard(BingoTypes::gameType game)
    : _game{game} {
    _victoryCondition = nullptr;
}

BingoCard::BingoCard(std::vector<Square*> grid, BingoTypes::gameType type)
    : BingoCard{type} {
    setGrid(grid);
}

BingoCard::~BingoCard() {
    // Deallocate any dynamically allocated memory
    for (auto square : _grid) {
        delete square;
    }
}

BingoTypes::gameType BingoCard::getGameType() {
    return _game;
}

BingoTypes::victoryType BingoCard::getVictoryType() {
    // Check if victory condition is set and use its victory type
    if (_victoryCondition) {
        return _victoryCondition->getVictoryType();
    } else {
        // Default to blackout if no victory condition set
        return BingoTypes::BLACKOUT;
    }
}

Square* BingoCard::getSquare(BingoTypes::squarePos pos) {
    // Validate position and access square from the grid
    unsigned location = squarePosToLocation(pos);
    if (location >= _grid.size()) {
        throw bad_input("Invalid square position entered.");
    }
    return _grid[location];
}

void BingoCard::setVictoryCondition(VictoryCondition* victory) {
    // Assign the victory condition
    _victoryCondition = victory;
}

void BingoCard::setGrid(std::vector<Square*> grid) {
  // Validate grid size
  if (grid.size() != 25) {
    throw bad_input("Invalid grid size. Bingo card requires 25 squares.");
  }
  if (!typesMatch(_game, grid)) {
    throw bad_input("Grid squares do not match the specified game type.");
  }

  checkGridValidity(grid);

  for (auto square : _grid) {
    delete square;
  }
  _grid = grid;
}

void BingoCard::resetCard() {
    // Mark all squares as undaubed
    for (auto square : _grid) {
        square->setDaubed(false);
        square->reset();
    }

}

bool BingoCard::daubSquare(unsigned numberCalled, BingoTypes::squarePos pos) {
  // Get the square at the specified position
  Square* square = getSquare(pos);
  // Check if the square matches the called number
  if (square->matchesNumber(numberCalled)) {
    square->daub(); // Mark the square as daubed
    return true;
  } else {
    return false;
  }
}

bool BingoCard::isCorrect() {
    // Check if all squares are correctly marked
    for (auto square : _grid) {
        if (!square->isCorrect()) {
            return false;
        }
    }
    return true;
}

bool BingoCard::isVictorious() {
    // Check if the victory condition is met
    if (_victoryCondition) {
        return _victoryCondition->checkVictory(_grid);
    } else {
        // Check for blackout if no victory condition set
        for (auto square : _grid) {
            if (!square->isDaubed()) {
                return false;
            }
        }
        return true;
    }
}

bool BingoCard::typesMatch(BingoTypes::gameType game,
                           BingoTypes::victoryType victory) {
  // Check if square types match the game type and victory condition
  switch (game) {
    case BingoTypes::BINGO50:
      // Only BINGO50Square allowed
      return (victory == BingoTypes::BLACKOUT ||
              dynamic_cast<Bingo50Square*>(getSquare({1, 1})) != nullptr);
    case BingoTypes::BINGO75:
      // Bingo75Square or FreeSquare allowed
      return true;
    default:
      throw bad_input("Invalid game type.");
  }
}

void BingoCard::checkGridValidity(std::vector<Square*> grid) {
    // Check if the grid size and square types are valid for the game type
    if (grid.size() != 25) {
        throw bad_input("Invalid grid size. Expected 25 squares.");
    }
    // Add additional checks based on specific game rules and square types
}

unsigned BingoCard::squarePosToLocation(BingoTypes::squarePos pos) {
    if (pos.row < 1 || pos.row > 5 || pos.col < 1 || pos.col > 5) {
        throw bad_input("Invalid square position entered.");
    }

    return (5 * (pos.col - 1) + (pos.row - 1));
}
