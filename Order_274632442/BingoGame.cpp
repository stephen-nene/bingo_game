
#include <cctype>
#include <string>

#include "BingoGame.h"
#include "BingoTypes.h"
#include "ScreenDisplay.h"
#include "UserInput.h"
#include "VictoryCondition.h"

#include "Exceptions.h"

BingoGame::BingoGame() {
  _caller = nullptr;
}

BingoGame::~BingoGame() {
    for (auto& player : _player) {
        delete player.second;
    }
    _player.clear();
}

BingoTypes::gameType BingoGame::getGameType() {
  if (_caller == nullptr) {
    throw incomplete_settings
    ("Bingo caller is not set, so game type is unknown.");
  }
  return _caller->getGameType();
}

BingoTypes::victoryType BingoGame::getVictoryType() {
  if (_caller == nullptr) {
    throw incomplete_settings
    ("Bingo caller is not set, so victory type is unknown.");
  }
  return _caller->getVictoryType();
}

unsigned BingoGame::getNumPlayers() {
  return _player.size();
}

void BingoGame::setCaller(BingoCaller* caller) {
  if (caller == nullptr) {
    throw bad_input("Caller cannot be a nullptr.");
  }

  _caller = caller;
}

void BingoGame::resetVictoryType(BingoTypes::victoryType victory) {
  if (_caller == nullptr) {
    throw incomplete_settings
    ("Bingo caller is not set, so victory type cannot be set.");
  }

  if (_caller->getNumBallsPulled() > 0) {
    throw function_unavailable
    ("The victory type cannot be changed after the game has begun.");
  }

  _caller->setVictoryType(victory);
}


void BingoGame::completeNextCall(std::ostream& out) {
    if (_caller == nullptr) {
        throw incomplete_settings("The caller hasn't been set.");
    }

    if (_player.empty()) {
        throw invalid_size("There are no players before the first turn.");
    }

    bool bingoCalled = false;
    for (auto& player : _player) {
        out << "Player: " << player.first << std::endl;
        _caller->pullBall();
        out << "Announcement: " << _caller->getAnnouncement() << std::endl;
        player.second->daubNumber(_caller->getCurrentNumber());

        if (player.second->isWinner()) {
            _winners.push_back(player.first);
            bingoCalled = true;
        }
    }

    if (bingoCalled || _player.size() == 0) {
        endGame(out);
    }
}

void BingoGame::takeAction(std::ostream& out, std::istream& in, std::string id,
                           BingoTypes::moveType move) {
  if (_caller == nullptr) {
    throw incomplete_settings
    ("Bingo caller is not set, cannot start the game.");
  }

  if (_player.empty()) {
    throw incomplete_settings
    ("No players added, cannot start the game.");
  }

  if (_player.find(id) == _player.end()) {
    throw invalid_identifier("Unknown identifier.");
  }

  switch (move) {
    case BingoTypes::DAUB :
      daubMove(out, in, id);
      break;
    case BingoTypes::BINGO :
      bingoMove(out, in, id);
      break;
    case BingoTypes::SHOW_CARD :
      showCardMove(out, in, id);
      break;
    case BingoTypes::CHECK_CARD :
      checkCardMove(out, in, id);
      break;
    case BingoTypes::SHOW_GAME :
      showGameMove(out, in, id);
      break;
    case BingoTypes::SHOW_BOARD :
      showBoardMove(out, in, id);
      break;
    case BingoTypes::HELP :
      helpMove(out, in, id);
      break;
    case BingoTypes::QUIT_GAME :
      quitGameMove(out, in, id);
      break;
    default :
      throw function_unavailable("This is not an available move.");
  }
}

void BingoGame::daubMove(std::ostream& out, std::istream& in,
                         std::string id) {
  ScreenDisplay screen;
  screen.displaySquarePositionPrompt(out);
  UserInput ui;
  BingoTypes::squarePos pos = ui.getSquarePosition(in);

  std::string msg = "Square (" + std::to_string(pos.row) + ", "
    + std::to_string(pos.col) + ") ";

  if (_player[id]->daubSquare(_caller->getCurrentNumber(), pos)) {
    msg += "has been daubed.\n";
  } else {
    msg += "is already daubed.\n";
  }

  screen.displayCallerMessage(out, msg);
}

void BingoGame::bingoMove(std::ostream& out, std::istream& in,
                          std::string id) {
  ScreenDisplay screen;
  std::string msg = id + ": Your card has ";
  if (_player[id]->isVictorious()) {
    _winners.push_back(id);
  }  else {
    msg += "not ";
  }
  msg += "met the victory conditions for this game.\n";
  screen.displayCallerMessage(out, msg);
}

void BingoGame::showCardMove(std::ostream& out, std::istream& in,
                             std::string id) {
  std::string msg = id + "\n";
  ScreenDisplay screen;
  screen.displayCallerMessage(out, msg);
  screen.displayBingoCard(out, _player[id]);
}

void BingoGame::checkCardMove(std::ostream& out, std::istream& in,
                             std::string id) {
  ScreenDisplay screen;
  screen.displayValidity(out, _player[id]);
  if (_player[id]->isCorrect()) {
    screen.displayCallerMessage(out, "No errors found on this card.\n");
  } else {
    screen.displayCallerMessage(out, "Errors found on this card.\n");
  }
}

void BingoGame::showGameMove(std::ostream& out, std::istream& in,
                             std::string id) {
  ScreenDisplay screen;
  screen.displayGameInfo(out, _caller);
}


void BingoGame::showBoardMove(std::ostream& out, std::istream& in, std::string id) {
    // Get the player's bingo card and display it
    auto it = _player.find(id);
    if (it != _player.end()) {
        BingoCard* card = it->second;
        ScreenDisplay::displayCard(out, card);
    } else {
        throw invalid_identifier("Player ID not found.");
    }
}

void BingoGame::helpMove(std::ostream& out, std::istream& in, std::string id) {
    // Display help instructions to the player
    out << "Welcome to Bingo Game Help!" << std::endl;
    out << "----------------------------------------------" << std::endl;
    out << "Here are some helpful instructions:" << std::endl;
    out << "- To mark a number on your card, enter 'D' followed by the number (e.g., D23)." << std::endl;
    out << "- To check if you have a winning pattern, enter 'B' to call Bingo." << std::endl;
    out << "- To display your bingo card, enter 'S'." << std::endl;
    out << "- To quit the game, enter 'Q'." << std::endl;
    out << "----------------------------------------------" << std::endl;
}

void BingoGame::quitGameMove(std::ostream& out, std::istream& in,
                             std::string id) {
  ScreenDisplay screen;
  screen.displayCallerMessage(out, id + " has quit the game.\n");
}


bool BingoGame::joinGame(std::string id, BingoCard* card) {
  unsigned count = 0;
  for (char ch : id) {
    if (isspace(ch)) {
      count++;
    }
  }
  if (id.length() == count) {
    throw invalid_identifier("The player id cannot be blank.");
  }

  if (card == nullptr) {
    throw bad_input("Card cannot be a nullptr.");
  }

  if (_caller == nullptr) {
    throw incomplete_settings
    ("Bingo caller is not set, players cannot join the game.");
  }

  if (!card->typesMatch(_caller->getGameType(), _caller->getVictoryType())) {
    throw card_to_game_mismatch("Cannot use this card with this game.");
  }

  if (_player.find(id) != _player.end()) {
    return false;
  }

  _player[id] = card;

  return true;
}

bool BingoGame::leaveGame(std::string id) {
    auto it = _player.find(id);
    if (it != _player.end()) {
        delete it->second;
        _player.erase(it);
        return true;
    }
    return false;
}

void BingoGame::endGame(std::ostream& out) {
    if (!_caller) {
        throw incomplete_settings("Caller not set.");
    }

    // Check for winners
    for (auto& pair : _player) {
        BingoCard* card = pair.second;
        if (card->checkForWin(_caller->getVictoryType())) {
            _winners.push_back(pair.first);
        }
    }

    // If there are winners or no more players, end the game
    if (!_winners.empty() || _player.empty()) {
        ScreenDisplay::displayWinners(out, _winners);
        // Reset the game after ending
        resetGame();
    }
}

void BingoGame::resetGame() {
    _winners.clear();
    for (auto& pair : _player) {
        delete pair.second;
    }
    _player.clear();
}
