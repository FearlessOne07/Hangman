#include "Game.hpp"
#include "Colors.hpp"
#include "Difficulty.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>

Game::Game(const std::vector<std::string> &words, Difficulty difficulty)
    : _words(words), _guessedLetters() {
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

  int length = _hangWord.size();

  std::cout << _hangWord << "\n";
  // State the Number of letters in the word
  std::cout << "This word has " << FG_YELLOW << UNDERLINE << length << RESET
            << " letters.\n";

  while (!_solved && _maxTries > 0) {

    // State number of tries left
    std::cout << "You have " << FG_YELLOW << UNDERLINE << _maxTries << RESET
              << " tries left.\n";

    // Stae the numbers of letters left to guess
    std::cout << FG_YELLOW << UNDERLINE << length << RESET
              << " letters left to guess.\n\n";

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

    // Print the Show string
    std::cout << _showString << "\n\n";

    // Ask user for guess
    std::cout << "Take your guess: ";

    // Get the user's guess
    std::cin >> _guess;
    std::cout << "\n";

    // Validate the guess
    _solved = CheckWord();

    std::cout << "------------------------------------------------\n\n";
  }

  if (_solved) {
    std::cout << "Yay! You got the word " << FG_GREEN << _hangWord << "\n";
    std::cout << RESET;
    std::cout << FG_BLUE << "Well Done!\n\n";
    std::cout << RESET;
  } else {
    std::cout << "Ooh! Togh Luck! The word  was" << FG_GREEN << _hangWord
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

  // Check if the guess is alphabetic
  if (!std::all_of(_guess.begin(), _guess.end(),
                   [](char c) { return std::isalpha(c); })) {
    std::cout << "You can only guess a " << FG_RED << UNDERLINE << "letter"
              << RESET << "\n\n";
    return false;
  }

  // Check if the guess is not one letter or is longer than the hangword
  if ((_guess.size() > 1 && _guess.size() < _hangWord.size()) ||
      _guess.size() > _hangWord.size()) {
    std::cout << FG_RED << "Your guess must be " << FG_YELLOW << UNDERLINE
              << "one letter" << FG_RED << "or a word " << FG_YELLOW
              << UNDERLINE << "equal to" << FG_RED
              << "the length of the word.\n\n";
    std::cout << RESET;
    return false;
  }

  // If the guess is the same length as the hangword
  if (_guess.size() == _hangWord.size()) {

    // If the guess is not the hang word
    if (_guess != _hangWord) {
      std::cout << FG_RED;
      std::cout << " Ooooh, that word wasn't quite right. Try "
                   "again.\033[0m \n\n";
      _maxTries--;
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
      std::cout << FG_MAGENTA << "Letter has already been guessed.\n\n";
      std::cout << RESET;
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

        // Subtract the found number of letter from the number of left
        // characters
        _charsLeft -= positions.size();
      } else {
        std::cout << FG_RED;
        std::cout
            << "Ooooh, that letter wasn't quite right. Try again.\033[0m\n\n";
        _maxTries--;
        return false;
      }

      // Check if show string is completley filled
      if (_showString.find("_") == std::string::npos) {
        return true;
      } else {
        std::cout << FG_BLUE << "Well done! You guessed the letter \""
                  << FG_GREEN << _guess << RESET << "\"\n\n";
        return false;
      }
    }
  }
  return false;
}
