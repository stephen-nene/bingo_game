
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "BingoCaller.h"
#include "BingoTypes.h"
#include "MakeRandomInt.h"
#include "Exceptions.h"

BingoCaller::BingoCaller(BingoTypes::victoryType victory)
  : _victory{victory} {
  _currentBall = _ballsChosen.end();
}

BingoCaller::~BingoCaller() {}

BingoTypes::victoryType BingoCaller::getVictoryType() {
  return _victory;
}

BingoTypes::gameType BingoCaller::getGameType() {
  return _game;
}

unsigned BingoCaller::getNumBalls() {
  return static_cast<unsigned>(_game);
}

unsigned BingoCaller::getNumBallsPulled() {
  return _ballsChosen.size();
}

unsigned BingoCaller::getNumBallsInCage() {
  return _ballCage.size();
}

std::string BingoCaller::listPulledBalls() {
// STUB
  return "";
}

std::string BingoCaller::listUnpulledBalls() {
// STUB
  return "";
}

void BingoCaller::setVictoryType(BingoTypes::victoryType victory) {
  _victory = victory;
}

bool BingoCaller::pullBall() {
// STUB
  return false;
}

unsigned BingoCaller::getCurrentNumber() {
// STUB
  return 0;
}

bool BingoCaller::wasNumberCalled(unsigned number) {
// STUB
  return false;
}

void BingoCaller::resetGame() {
// STUB
}

std::string BingoCaller::makeList(const std::vector<unsigned>& balls) {
// STUB
  return "";
}

char BingoCaller::getLetter(unsigned value) {
// STUB
  return 'B';
}

Bingo50Caller::Bingo50Caller(BingoTypes::victoryType victory)
  : Bingo50Caller{"data/bingo50calls.csv", victory} {}

Bingo50Caller::Bingo50Caller(std::string cluesFile,
                             BingoTypes::victoryType victory)
  : BingoCaller{victory} {
// STUB
}

Bingo50Caller::~Bingo50Caller() {
// STUB
}

std::string Bingo50Caller::getAnnouncement() {
// STUB
  return "";
}

void Bingo50Caller::readClues(std::string cluesFile) {
// STUB
}

Bingo75Caller::Bingo75Caller(BingoTypes::victoryType victory)
  : BingoCaller{victory} {
// STUB
}

Bingo75Caller::~Bingo75Caller() {}

std::string Bingo75Caller::getAnnouncement() {
// STUB
  return "";
}
