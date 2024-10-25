#include "Difficulty.hpp"
#include "Game.hpp"
#include "Utility.hpp"
#include <iostream>
#include <unordered_map>

int main(void) {
  // Initialize variables
  std::unordered_map<Difficulty, std::vector<std::string>> words;
  Difficulty difficulty;
  bool wantsPlay = true;

  // Load Words
  if (!LoadWords(words)) {
    return -1;
  }

  //
  while (wantsPlay) {
    difficulty = ChooseDifficulty();
    Game game = Game(words[difficulty], difficulty);
    game.Run();

    // Query Player to playe again
    std::string play;
    std::cout << "Press 'P' to play again or 'E' to quit.\n";
    std::cin >> play;

    if (std::tolower(play[0]) == 'p') {
      continue;
    } else {
      wantsPlay = false;
    }
  }

  return 0;
}
