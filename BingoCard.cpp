
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
    // Destructor implementation
    for (auto square : _grid) {
        delete square;
    }
    _grid.clear();
    delete _victoryCondition;
}

BingoTypes::gameType BingoCard::getGameType() {
  return _game;
}

BingoTypes::victoryType BingoCard::getVictoryType() {
    // Getter for victory type
    if (_victoryCondition != nullptr) {
        return _victoryCondition->getType();
    } else {
        return BingoTypes::BLACKOUT; // Default value
    }
}

Square* BingoCard::getSquare(BingoTypes::squarePos pos) {
    // Getter for a specific square
    unsigned location = squarePosToLocation(pos);
    if (location < _grid.size()) {
        return _grid[location];
    } else {
        return nullptr; // Invalid position
    }
}

void BingoCard::setVictoryCondition(VictoryCondition* victory) {
    delete _victoryCondition; // Release previous victory condition if any
    _victoryCondition = victory;
}

void BingoCard::setGrid(std::vector<Square*> grid) {
    _grid = grid;
}

void BingoCard::resetCard() {
    for (auto square : _grid) {
        square->setDaubed(false); // Reset daub state for all squares
    }
}

bool BingoCard::daubSquare(unsigned numberCalled, BingoTypes::squarePos pos) {
    Square* square = getSquare(pos);
    if (square != nullptr) {
        square->setDaubed(true); // Mark square as daubed
        return true;
    }
    return false; // Return false if square not found
}

bool BingoCard::isCorrect() {
    for (auto square : _grid) {
        if (!square->isCorrect()) {
            return false; // Return false if any square is not correct
        }
    }
    return true; // All squares are correct
}

bool BingoCard::isVictorious() {
    if (_victoryCondition != nullptr) {
        return _victoryCondition->isVictorious(_grid); // Check victory condition
    }
    return false; // No victory condition set
}

bool BingoCard::typesMatch(BingoTypes::gameType game, BingoTypes::victoryType victory) {

    return (_game == game && getVictoryType() == victory);
}

void BingoCard::checkGridValidity(std::vector<Square*> grid) {
    // Check grid validity here if needed
    // Ensure the grid has exactly 25 squares
    if (grid.size() != 25) {
        throw invalid_grid("Grid must contain exactly 25 squares.");
    }

    // Ensure each square in the grid is unique
    std::vector<Square*> uniqueSquares = grid;
    std::sort(uniqueSquares.begin(), uniqueSquares.end());
    auto last = std::unique(uniqueSquares.begin(), uniqueSquares.end());
    if (last != uniqueSquares.end()) {
        throw invalid_grid("Grid contains duplicate squares.");
    }

    // Ensure each square is within the valid range
    for (auto square : grid) {
        if (!square->isValid()) {
            throw invalid_grid("Invalid square found in the grid.");
        }
    }
}

unsigned BingoCard::squarePosToLocation(BingoTypes::squarePos pos) {
  if (pos.row < 1 || pos.row > 5 || pos.col < 1 || pos.col > 5)
    throw bad_input("Invalid square position entered.");

  return (5 * (pos.col - 1) + (pos.row - 1));
}
