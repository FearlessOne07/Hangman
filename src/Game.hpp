#pragma once
#include "Difficulty.hpp"
#include <string>
#include <vector>

class Game {
private:
  const std::vector<std::string> &_words;
  std::vector<std::string> _guessedLetters;

  // Playing
  std::string _hangWord;
  std::string _showString;
  std::string _guess;
  int _charsLeft;
  int _maxTries = 0;
  bool _solved = false;

  // Helper Methods
  void ChooseWord();
  bool CheckWord();

public:
  Game(const std::vector<std::string> &words, Difficulty difficulty);
  Game();
  void Run();
};
