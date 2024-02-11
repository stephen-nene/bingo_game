
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
  std::string result;
  for (const auto& ball : _ballsChosen) {
    result += std::to_string(ball.number) + " ";
  }
  return result;
}

std::string BingoCaller::listUnpulledBalls() {
  std::string result;
  for (const auto& ball : _ballCage) {
    result += std::to_string(ball.number) + " ";
  }
  return result;
}

void BingoCaller::setVictoryType(BingoTypes::victoryType victory) {
  _victory = victory;
}

bool BingoCaller::pullBall() {
  if (_ballCage.empty()) {
    throw BingoException("The ball cage is empty.");
  }
  auto it = std::next(_ballCage.begin(), MakeRandomInt{0, static_cast<int>(_ballCage.size() - 1)}());
  _currentBall = _ballsChosen.insert(_ballsChosen.end(), *it);
  _ballCage.erase(it);
  return true;
}

unsigned BingoCaller::getCurrentNumber() {
  if (_currentBall == _ballsChosen.end()) {
    throw BingoException("No ball has been pulled yet.");
  }
  return _currentBall->number;
}

bool BingoCaller::wasNumberCalled(unsigned number) {
  for (const auto& ball : _ballsChosen) {
    if (ball.number == number) {
      return true;
    }
  }
  return false;
}

void BingoCaller::resetGame() {
  _ballsChosen.clear();
  _ballCage.clear();
  _currentBall = _ballsChosen.end();
}


std::string BingoCaller::makeList(const std::vector<unsigned>& balls) {
  std::string result;
  for (const auto& ball : balls) {
    result += std::to_string(ball) + " ";
  }
  return result;
}

char BingoCaller::getLetter(unsigned value) {
  // Assuming letters B, I, N, G, O correspond to ball ranges
  if (value >= 1 && value <= 15) return 'B';
  if (value >= 16 && value <= 30) return 'I';
  if (value >= 31 && value <= 45) return 'N';
  if (value >= 46 && value <= 60) return 'G';
  if (value >= 61 && value <= 75) return 'O';
  throw BingoException("Invalid ball value.");
}


Bingo50Caller::Bingo50Caller(BingoTypes::victoryType victory)
  : Bingo50Caller{"data/bingo50calls.csv", victory} {}

Bingo50Caller::Bingo50Caller(std::string cluesFile,
                             BingoTypes::victoryType victory)
  : BingoCaller{victory} {
  readClues(cluesFile);
}

Bingo50Caller::~Bingo50Caller() {
  // Destructor implementation
}

std::string Bingo50Caller::getAnnouncement() {
  // Implement announcement logic for Bingo50 game
  return "Welcome to Bingo50!";
}

void Bingo50Caller::readClues(std::string cluesFile) {
  // Implement reading clues from file for Bingo50 game
}

Bingo75Caller::Bingo75Caller(BingoTypes::victoryType victory)
  : BingoCaller{victory} {
  // Constructor implementation for Bingo75 game
}

Bingo75Caller::~Bingo75Caller() {
  // Destructor implementation for Bingo75 game
}

std::string Bingo75Caller::getAnnouncement() {
  // Implement announcement logic for Bingo75 game
  return "Welcome to Bingo75!";
}
