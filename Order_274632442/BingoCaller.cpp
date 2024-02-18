
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
  std::string list;
  if (!_ballsChosen.empty()) {
    for (auto it = _ballsChosen.begin(); it != _ballsChosen.end(); ++it) {
      if (it != _ballsChosen.begin()) {
        list += ", ";
      }
      list += makeList({*it});
    }
  }
  return list;
}

std::string BingoCaller::listUnpulledBalls() {
  std::string list;
  if (!_ballCage.empty()) {
    for (auto it = _ballCage.begin(); it != _ballCage.end(); ++it) {
      if (it != _ballCage.begin()) {
        list += ", ";
      }
      list += makeList({*it});
    }
  }
  return list;
}


void BingoCaller::setVictoryType(BingoTypes::victoryType victory) {
  _victory = victory;
}

bool BingoCaller::pullBall() {
  if (_ballCage.empty()) {
    return false;
  }

  // Use MakeRandomInt to get a random index
  auto index = MakeRandomInt::getInstance().getValue(_ballCage.size());

  // Use iterator to avoid invalidating iterators
  auto it = _ballCage.begin() + index;
  unsigned pulledBall = *it;

  _ballsChosen.push_back(pulledBall);
  _ballCage.erase(it);

  _currentBall = _ballsChosen.end();

  return true;
}

unsigned BingoCaller::getCurrentNumber() {
  if (_currentBall == _ballsChosen.end()) {
    throw std::invalid_size("No balls pulled yet.");
  }
  return *_currentBall;
}


bool BingoCaller::wasNumberCalled(unsigned number) {
  return std::find(_ballsChosen.begin(), _ballsChosen.end(), number) != _ballsChosen.end();
}

void BingoCaller::resetGame() {
  _ballsChosen.clear();
  _ballCage.clear();
  for (unsigned i = 1; i <= _game; ++i) {
    _ballCage.push_back(i);
  }
  _currentBall = _ballsChosen.end();
}


Bingo50Caller::Bingo50Caller(std::string cluesFile, BingoTypes::victoryType victory)
    : BingoCaller(victory) {
    // Fill _ballCage with all valid balls for BINGO50
    for (unsigned i = 1; i <= 50; ++i) {
        _ballCage.push_back(i);
    }
    _game = BingoTypes::BINGO50;
    _description = "Bingo 50";
    readClues(cluesFile);
}


char Bingo50Caller::getLetter(unsigned value) {
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

Bingo50Caller::Bingo50Caller(std::string cluesFile, BingoTypes::victoryType victory)
  : BingoCaller{victory} {
  // Initialize _ballCage with all valid balls for BINGO50
  for (unsigned i = 1; i <= 50; ++i) {
    _ballCage.push_back(i);
  }
  _game = BingoTypes::BINGO50;
  _description = "Bingo 50";

  // Ensure a default clues file is provided, otherwise throw an exception
  if (cluesFile.empty()) {
    throw BingoException("Clues file cannot be empty.");
  }

  // Call readClues to populate clues from the specified file
  readClues(cluesFile);
}


Bingo50Caller::~Bingo50Caller() {
  if (_clues != nullptr) {
    for (unsigned i = 0; i < 50; ++i) { // Assuming 50 values due to lack of information
      delete[] _clues[i];
    }
    delete[] _clues;
  }
  // Deallocate memory for the numClues array
  delete[] _numClues;
}


std::string Bingo50Caller::getAnnouncement() {
  // Implement announcement logic for Bingo50 game
  if (_currentBall == _ballsChosen.end()) {
    throw BingoException("No ball has been pulled yet.");
  }

  // Randomly select an announcement from the clues for the pulled ball
  unsigned ball = *_currentBall;
  int randomClueIndex = MakeRandomInt::getInstance().getValue(_numClues[ball - 1]);
  return _clues[ball - 1][randomClueIndex];
}

void Bingo50Caller::readClues(std::string cluesFile) {
  // Implement reading clues from file for Bingo50 game
  std::ifstream file(cluesFile);
  if (!file.is_open()) {
    throw BingoException("Could not open clues file: " + cluesFile);
  }

  // Read the number of values and clues per value
  unsigned numValues, numClues;
  file >> numValues >> numClues;
  if (numValues != 50 || numClues < 1 || numClues > 10) {
    throw BingoException("Invalid format in clues file: " + cluesFile);
  }

  // Allocate memory for the clues array
  _clues = new std::string*[numValues];
  for (unsigned i = 0; i < numValues; ++i) {
    _clues[i] = new std::string[numClues];
  }
  _numClues = new unsigned[numValues];

  // Read the clues for each value
  for (unsigned i = 0; i < numValues; ++i) {
    unsigned value;
    file >> value;
    if (value != i + 1) {
      throw BingoException("Invalid format in clues file: " + cluesFile);
    }
    _numClues[i] = numClues;
    for (unsigned j = 0; j < numClues; ++j) {
      std::string clue;
      std::getline(file, clue); // Read entire line including spaces
      _clues[i][j] = clue;
    }
  }

  file.close();
}

Bingo75Caller::Bingo75Caller(BingoTypes::victoryType victory)
  : BingoCaller{victory} {
  // Fill _ballCage with all valid balls for BINGO75
  for (unsigned i = 1; i <= 75; ++i) {
    _ballCage.push_back(i);
  }
  _game = BingoTypes::BINGO75;
  _description = "Bingo 75";
}

std::string Bingo75Caller::getAnnouncement() {
  // Implement announcement logic for Bingo75 game
  if (_currentBall == _ballsChosen.end()) {
    throw BingoException("No ball has been pulled yet.");
  }

  unsigned ball = *_currentBall;
  char letter = getLetter(ball);
  return "Letter " + std::to_string(letter) + ", Number " + std::to_string(ball);
}

Bingo75Caller::~Bingo75Caller() {
  // No specific cleanup needed for Bingo75Caller
}

std::string Bingo75Caller::getAnnouncement() {
  // Implement announcement logic for Bingo75 game
  if (_currentBall == _ballsChosen.end()) {
    throw BingoException("No ball has been pulled yet.");
  }

  unsigned ball = *_currentBall;
  char letter = getLetter(ball);
  return "Letter " + std::to_string(letter) + ", Number " + std::to_string(ball);
}