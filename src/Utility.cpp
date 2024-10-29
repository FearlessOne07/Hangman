#include "Utility.hpp"
#include "Colors.hpp"
#include <fstream>
#include <iostream>

bool LoadWords(
    std::unordered_map<Difficulty, std::vector<std::string>> &words) {

  Difficulty difficulty;
  std::fstream file;
  std::string word;

  file.open("words.txt");
  if (!file) {
    std::cout
        << "Failed to find \"words.txt\". Make sure it is in the same working "
           "directory as the \"Hangman\" executable.\n";
    return false;
  }

  // Read File
  while (std::getline(file, word)) {
    if (word.find("Easy") != std::string::npos) {
      difficulty = Difficulty::EASY;
      continue;
    } else if (word.find("Medium") != std::string::npos) {
      difficulty = Difficulty::MEDIUM;
      continue;
    } else if (word.find("Hard") != std::string::npos) {
      difficulty = Difficulty::HARD;
      continue;
    } else if (word.find("Dictionary") != std::string::npos) {
      difficulty = Difficulty::DICTIONARY;
      continue;
    }
    words[difficulty].push_back(word);
  }
  std::cout << FG_MAGENTA;
  std::cout << "Done reading words file\n\n";
  std::cout << RESET;
  return true;
}

Difficulty ChooseDifficulty() {
  unsigned short choice;
  Difficulty difficulty;
  bool chosen = false;

  std::cout << FG_GREEN << "1. Easy\n" << RESET;
  std::cout << FG_YELLOW << "2. Medium\n" << RESET;
  std::cout << FG_BLUE << "3. Hard\n" << RESET;
  std::cout << FG_RED << "4. Dictionary\n" << RESET;

  while (!chosen) {
    std::cout << FG_MAGENTA;
    std::cout << "Choose a difficulty: ";
    std::cin >> choice;
    std::cout << RESET;
    if (!choice || !(choice > 0 || choice < 5)) {
      continue;
    }

    chosen = true;
    difficulty = static_cast<Difficulty>(choice);
    if (difficulty == Difficulty::EASY) {
      std::cout << FG_GREEN;
      std::cout << "Your chosen Diffuculty is Easy\n";
      std::cout << RESET;
    } else if (difficulty == Difficulty::MEDIUM) {

      std::cout << FG_YELLOW;
      std::cout << "Your chosen Diffuculty is Medium\n";
      std::cout << RESET;
    } else if (difficulty == Difficulty::HARD) {
      std::cout << FG_BLUE;
      std::cout << "Your chosen Diffuculty is Hard\n";
      std::cout << RESET;
    } else if (difficulty == Difficulty::DICTIONARY) {
      std::cout << FG_RED;
      std::cout << "Your chosen Diffuculty is Dictionary\n";
      std::cout << RESET;
    } else {
      std::cout << "Invalid choice. Try again\n";
      std::cin.clear();
      std::cin.ignore();
      chosen = false;
    }
  }
  std::cout << "\n----------------------------------------------\n\n";
  return difficulty;
}
