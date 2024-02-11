
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
// STUB
}

BingoTypes::gameType BingoCard::getGameType() {
  return _game;
}

BingoTypes::victoryType BingoCard::getVictoryType() {
// STUB
  return BingoTypes::BLACKOUT;
}

Square* BingoCard::getSquare(BingoTypes::squarePos pos) {
// STUB
  return new FreeSquare();
}

void BingoCard::setVictoryCondition(VictoryCondition* victory) {
// STUB
  _victoryCondition = victory;
}

void BingoCard::setGrid(std::vector<Square*> grid) {
// STUB
  _grid = grid;
}

void BingoCard::resetCard() {
// STUB
}

bool BingoCard::daubSquare(unsigned numberCalled, BingoTypes::squarePos pos) {
// STUB
  return false;
}

bool BingoCard::isCorrect() {
// STUB
  return false;
}

bool BingoCard::isVictorious() {
// STUB
  return false;
}

bool BingoCard::typesMatch(BingoTypes::gameType game,
                           BingoTypes::victoryType victory) {
// STUB
    return false;
}

void BingoCard::checkGridValidity(std::vector<Square*> grid) {
// STUB
}

unsigned BingoCard::squarePosToLocation(BingoTypes::squarePos pos) {
  if (pos.row < 1 || pos.row > 5 || pos.col < 1 || pos.col > 5)
    throw bad_input("Invalid square position entered.");

  return (5 * (pos.col - 1) + (pos.row - 1));
}
