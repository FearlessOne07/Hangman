
#include <iostream>

int main() {
  std::string guess = "test";
  std::string hangWord = "example";

  if (hangWord.find(guess) == std::string::npos) {
    std::cout << "\033[31m";
    std::cout << "Ooooh, that word wasn't quite right. Try again.\033[0m\n\n";
  } else {
    std::cout << "You guessed the word!\n";
  }

  return 0;
}
