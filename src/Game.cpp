#include "Game.hpp"
#include "Colors.hpp"
#include "Difficulty.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>

Game::Game(const std::vector<std::string> &words, Difficulty difficulty)
    : _words(words), _guessedLetters(), _difficulty(difficulty) {
  switch (difficulty) {
  case Difficulty::EASY:
    _maxTries = 6;
    break;
  case Difficulty::MEDIUM:
    _maxTries = 9;
    break;
  case Difficulty::HARD:
    _maxTries = 10;
    break;
  case Difficulty::DICTIONARY:
    _maxTries = 12;
    break;
  default:
    _maxTries = 0;
    break;
  }

  _triesLeft = _maxTries;
  std::string _hangWord = "";
  std::string _showString = "";
  std::string _guess = "";
}

void Game::Run() {

  // Select A word
  ChooseWord();

  // Initialize Show String
  for (int i = 0; i < _hangWord.size(); ++i) {
    _showString += "_";
  }

  _charsLeft = _hangWord.length();

  while (!_solved && _triesLeft > 0) {

    RenderScreen();
    // Ask user for guess
    std::cout << "Take your guess: ";

    // Get the user's guess
    std::cin >> _guess;
    std::cout << "\n";

    // Validate the guess
    _solved = CheckWord();
  }

  if (_solved) {
    std::cout << "Yay! You got the word " << FG_GREEN << _hangWord << "\n";
    std::cout << RESET;
    std::cout << FG_BLUE << "Well Done!\n\n";
    std::cout << RESET;
  } else {
    std::cout << "Ooh! Togh Luck! The word  was " << FG_GREEN << _hangWord
              << "\n";
    std::cout << RESET;
    std::cout << FG_MAGENTA << "Better Luck Next Time\n\n";
    std::cout << RESET;
  }
}

void Game::ChooseWord() {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<int> dist(0, _words.size() - 1);
  _hangWord = _words[dist(gen)];
}

bool Game::CheckWord() {
  std::vector<std::size_t> positions;
  _status = "";

  // Check if the guess is alphabetic
  if (!std::all_of(_guess.begin(), _guess.end(),
                   [](char c) { return std::isalpha(c); })) {
    _status =
        "You can only guess a " + FG_RED + UNDERLINE + "letter" + RESET + "";

    //////////////////
    std::cout << BOLD << FG_BRIGHT_MAGENTA << UNDERLINE
              << "Jessica Tidmarsh, My beautiful baby girl!!!!\n\n"
              << RESET;
    //////////////////

    return false;
  }

  // Check if the guess is not one letter or is longer than the hangword
  if ((_guess.size() > 1 && _guess.size() < _hangWord.size()) ||
      _guess.size() > _hangWord.size()) {
    _status = FG_RED + "Your guess must be " + FG_YELLOW + UNDERLINE +
              "one letter" + RESET + FG_RED + " or a word " + FG_YELLOW +
              UNDERLINE + "equal to" + RESET + FG_RED +
              " the length of the word." + RESET;
    return false;
  }

  // If the guess is the same length as the hangword
  if (_guess.size() == _hangWord.size()) {

    // If the guess is not the hang word
    if (_guess != _hangWord) {
      std::cout << FG_RED;
      _status = " Ooooh, that word wasn't quite right. Try "
                "again." +
                RESET;
      _triesLeft--;
      return false;
    } else {

      // Set the show string to the guess and return true
      _showString = _guess;
      return true;
    }
  } else if (_guess.size() == 1) {

    // Check of the letter has already been guessed
    if (std::find(_guessedLetters.begin(), _guessedLetters.end(), _guess) !=
        _guessedLetters.end()) {
      _status = FG_MAGENTA + "Letter has already been guessed." + RESET;
      return false;
    } else if (_showString.find(_guess[0]) == std::string::npos) {
      std::size_t position = _hangWord.find(_guess);

      // Add the guess to the list of guessed letters
      _guessedLetters.push_back(_guess);

      if (position != std::string::npos) {

        // Find the positions of the guess in the hangword
        while (position != std::string::npos) {
          positions.push_back(position);
          position = _hangWord.find(_guess, position + 1);
        }

        // Replace all the letters at the fond positions in the show string with
        // the guess
        for (std::size_t pos : positions) {
          _showString[pos] = _guess[0];
        }

        // Subtract the found number of letters from the number of left
        // characters
        _charsLeft -= positions.size();
      } else {
        std::cout << FG_RED;
        _status = "Ooooh, that letter wasn't quite right. Try again." + RESET;
        _triesLeft--;
        return false;
      }

      // Check if show string is completley filled
      if (_showString.find("_") == std::string::npos) {
        return true;
      } else {
        _status = FG_BLUE + "Well done! You guessed the letter" + FG_GREEN +
                  _guess + FG_BLUE + RESET;
        return false;
      }
    }
  }
  return false;
}

void Game::RenderScreen() {
  std::cout << RESET_CURSOR_AND_CLEAR;
  std::cout
      << FG_MAGENTA +
             "══════════════════════════════════════════════════════════\n" +
             RESET;
  std::cout << FG_MAGENTA + "Difficulty: "
            << (_difficulty == Difficulty::EASY     ? FG_GREEN + "Easy"
                : _difficulty == Difficulty::MEDIUM ? FG_YELLOW + "Medium"
                : _difficulty == Difficulty::HARD   ? FG_BLUE + "Hard"
                                                    : FG_RED + "Dictionary")
            << "\n"
            << RESET;

  // State the Number of letters in the word
  std::cout << FG_MAGENTA << "Letters in word: " << FG_YELLOW
            << _hangWord.length() << RESET << "\n";

  // Stae the numbers of letters left to guess
  std::cout << FG_MAGENTA << "Letters left to guess: ";
  if (_triesLeft >= static_cast<int>(_maxTries * (70 / 100))) {
    std::cout << FG_RED;
  } else if (_triesLeft >= static_cast<int>(_maxTries * (40 / 100))) {
    std::cout << FG_YELLOW;
  } else if (_triesLeft >= static_cast<int>(_maxTries * (30 / 100))) {
    std::cout << FG_GREEN;
  }
  std::cout << _charsLeft << RESET << "\n";

  // State number of tries left
  std::cout << FG_MAGENTA << "Tries left: ";
  if (_triesLeft >= static_cast<int>(_maxTries * (70 / 100))) {
    std::cout << FG_GREEN;
  } else if (_triesLeft >= static_cast<int>(_maxTries * (40 / 100))) {
    std::cout << FG_YELLOW;
  } else if (_triesLeft >= static_cast<int>(_maxTries * (30 / 100))) {
    std::cout << FG_RED;
  }

  std::cout << _triesLeft << RESET << "\n";

  // Show the letters already tried
  std::cout << "Letters tried: [";
  for (std::string &letter : _guessedLetters) {
    if (_guessedLetters[0] != letter) {
      std::cout << ", ";
    }

    if (_hangWord.find(letter) != std::string::npos) {
      std::cout << FG_GREEN;
    } else {
      std::cout << FG_RED;
    }

    std::cout << letter;
    std::cout << RESET;
  }
  std::cout << "]\n\n";

  // Print status for player
  std::cout << _status << "\n";

  // Print the Show string
  std::cout << FG_BRIGHT_MAGENTA << _showString << "\n\n";
  std::cout << RESET;
}
