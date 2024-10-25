#include "Utility.hpp"
#include <fstream>
#include <iostream>

bool LoadWords(
    std::unordered_map<Difficulty, std::vector<std::string>> &words) {

  Difficulty difficulty;
  std::fstream file;
  std::string word;

  file.open("words.txt");
  if (!file) {
    std::cout << "Failed to find \"words.txt\". Make sure it is in the same "
                 "directory as the \"Hangman\" executable.\n";
    return false;
  }

  // Read File
  while (std::getline(file, word)) {
    if (word.find("Easy") != std::string::npos) {
      difficulty = Difficulty::EASY;
      std::cout << "Reading \"Easy\" words...\n";
      continue;
    } else if (word.find("Medium") != std::string::npos) {
      difficulty = Difficulty::MEDIUM;
      std::cout << "Reading \"Medium\" words...\n";
      continue;
    } else if (word.find("Hard") != std::string::npos) {
      difficulty = Difficulty::HARD;
      std::cout << "Reading \"Hard\" words...\n";
      continue;
    } else if (word.find("Dictionary") != std::string::npos) {
      difficulty = Difficulty::DICTIONARY;
      std::cout << "Reading \"Dictionary\" words...\n";
      continue;
    }
    words[difficulty].push_back(word);
  }

  std::cout << "Done reading file.\n\n";
  return true;
}

Difficulty ChooseDifficulty() {
  unsigned short choice;
  Difficulty difficulty;
  bool chosen = false;

  std::cout << "1 - Easy\n2 - Medium\n3 - Hard\n4 - Dictionary\n";
  while (!chosen) {
    std::cout << "Choose a difficulty: ";
    std::cin >> choice;

    chosen = true;
    difficulty = static_cast<Difficulty>(choice);
    if (difficulty == Difficulty::EASY) {
      std::cout << "Your chosen Diffuculty is Easy\n";
    } else if (difficulty == Difficulty::MEDIUM) {
      std::cout << "Your chosen Diffuculty is Medium\n";
    } else if (difficulty == Difficulty::HARD) {
      std::cout << "Your chosen Diffuculty is Hard\n";
    } else if (difficulty == Difficulty::DICTIONARY) {
      std::cout << "Your chosen Diffuculty is Dictionary\n";
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
