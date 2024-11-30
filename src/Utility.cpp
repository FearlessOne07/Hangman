#include "Utility.hpp"
#include "Colors.hpp"
#include <cctype>
#include <chrono>
#include <exception>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <thread>

// Helper

void ReadWordsFile(
    std::unordered_map<Difficulty, std::vector<std::string>> &words,
    std::promise<bool> &promise) {

  Difficulty difficulty;
  std::fstream file;
  std::string word;

  file.open("words.txt");
  if (!file) {
    std::cout << FG_RED;
    std::cout
        << "Failed to find \"words.txt\". Make sure it is in the same working "
           "directory as the \"Hangman\" executable.\n";
    std::cout << RESET;
    promise.set_value(false);
    return;
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
  std::cout << RESET;
  promise.set_value(true);
}

void PrintStatus() {
  for (int i = 0; i < 3; i++) {
    std::cout << FG_MAGENTA << BOLD;
    std::cout << "Reading Words file";

    // Print the dots according to the current iteration
    for (int j = 0; j <= i; j++) {
      std::cout << ".";
    }

    std::cout << "\r"; // Move the cursor back to the start of the line
    std::cout.flush(); // Ensure the output is immediately displayed
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  }
  std::cout << "Reading Words file... Done!\n"; // Final message
  std::cout << RESET;
}

bool LoadWords(
    std::unordered_map<Difficulty, std::vector<std::string>> &words) {

  std::promise<bool> promise;
  std::future<bool> result = promise.get_future();

  std::thread readFileThread(ReadWordsFile, std::ref(words), std::ref(promise));
  std::thread printStatusThread(PrintStatus);

  bool success;
  try {
    readFileThread.join();
    success = result.get();
  } catch (std::exception &e) {
    std::cerr << e.what() << "\n";
    return false;
  }
  printStatusThread.join();
  return success;
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

    if (!(std::cin >> choice) || choice < 1 || choice > 4) {
      std::cout << FG_RED;
      std::cout << "Invalid choice. Try again\n";
      std::cout << RESET;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
  std::cout << "\n----------------------------------------------\n\n";

  return difficulty;
}

void SplashScreen() {
  std::cout << RESET_CURSOR_AND_CLEAR;
  std::cout << FG_MAGENTA;
  std::cout << "╔════════════════════════════════════════╗\n";
  std::cout << "║            WELCOME TO HANGMAN          ║\n";
  std::cout << "╚════════════════════════════════════════╝\n" << RESET;
}
