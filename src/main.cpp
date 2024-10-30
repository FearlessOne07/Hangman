#include "Colors.hpp"
#include "Difficulty.hpp"
#include "Game.hpp"
#include "Utility.hpp"
#include <chrono>
#include <iostream>
#include <thread>
#include <unordered_map>

int main(void) {
  // Initialize variables
  std::cout << RESET_CURSOR_AND_CLEAR;
  std::unordered_map<Difficulty, std::vector<std::string>> words;
  Difficulty difficulty;
  bool wantsToPlay = true;
  bool validInput = false;

  // Load Words
  if (!LoadWords(words)) {
    return -1;
  }
  std::this_thread::sleep_for(std::chrono::seconds(2));

  while (wantsToPlay) {

    SplashScreen();
    difficulty = ChooseDifficulty();
    Game game = Game(words[difficulty], difficulty);
    game.Run();

    // Query Player to play again
    while (!validInput) {

      std::string input;
      std::cout << "Play Again? (P to play again, E to exit): ";
      std::cin >> input;
      if (input.empty()) {
        continue;
      }

      char choice = std::tolower(input[0]);
      if (choice == 'p') {
        validInput = true;
        wantsToPlay = true;
      } else if (choice == 'e') {
        validInput = true;
        wantsToPlay = false;
      } else {
        std::cout
            << "Invalid choice. Please enter 'P' to play or 'E' to exit.\n";
      }
    }
  }

  return 0;
}
