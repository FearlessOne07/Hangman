#include <cctype>
#include <cstddef>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>

// Custom Types
enum Difficulty : unsigned short
{
  EASY = 1,
  MEDIUM,
  HARD,
  DICTIONARY
};

// Functions
Difficulty ChooseDifficulty();
bool LoadWords(std::unordered_map<unsigned short, std::vector<std::string>>& words);
void BeginGame(
  std::unordered_map<unsigned short, std::vector<std::string>>&,
  Difficulty
);
void ChooseWord(std::vector<std::string>&, std::string&);
bool CheckWord(std::string& guess, std::string& showString, std::string& hangword, int&, int&);

int main(void)
{
  std::unordered_map<unsigned short, std::vector<std::string>> words;
  Difficulty difficulty;
  bool wantsPlay = true;

  if(!LoadWords(words))
  {
    return -1;
  }

  while(wantsPlay)
  {

    difficulty = ChooseDifficulty();
    BeginGame(words, difficulty);

    std::string play;

    std::cout << "Press 'P' to play again or 'E' to quit.\n";
    std::cin >> play;

    if(std::tolower(play[0]) == 'p')
    {
      continue;
    }
    else
    {
      wantsPlay = false; 
    }
  }

  return 0;
}

void ChooseWord(std::vector<std::string>& words, std::string& hangWord)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  
  std::uniform_int_distribution<int> dist(0, words.size() - 1);
  hangWord = words[dist(gen)];
}


bool CheckWord(std::string& guess, std::string& showString, std::string& hangWord, int& maxTries, int& length)
{
  std::vector<std::size_t> positions;
  if((guess.size() > 1 && guess.size() < hangWord.size()) || guess.size() > hangWord.size())
  {
    std::cout << "Your guess must be a letter or a word equal to the number of dashes.\n\n";
    return false;
  }

  if(guess.size() == hangWord.size())
  {
    if(hangWord.find(guess) == std::string::npos)
    {
      std::cout << "Ooooh, that word wasn't quite right. Try again.\n\n";
      maxTries--;
      return false;
    }
    else 
    {
      showString = guess;
      return true;
    }
  }

  if (guess.size() == 1) 
  {

    if(showString.find(guess[0]) == std::string::npos)
    {
      std::size_t position = hangWord.find(guess);

      if(position != std::string::npos)
      {

        while(position != std::string::npos)
        {
          positions.push_back(position);
          position = hangWord.find(guess, position + 1);
        }

        for(std::size_t pos : positions)
        {
          showString[pos] = guess[0]; 
        }
        length -= positions.size();
      }
      else 
      {
        std::cout << "Ooooh, that letter wasn't quite right. Try again.\n\n";
        maxTries--;
        return false;
      }

      if(showString.find("_") == std::string::npos)
      {
        return true;
      }
      else 
      { 
        std::cout << "Well done! You guessed the letter \"" << guess << "\"\n\n";
      }
    }
    else 
    {
      std::cout << "Letter has already been guessed.\n\n";
      return false;
    }
  }
  return false;
}

bool LoadWords(std::unordered_map<unsigned short, std::vector<std::string>>& words)
{
  Difficulty difficulty;
  std::fstream file;
  std::string word;

  file.open("words.txt");
  if(!file)
  {
    std::cout << "Failed to find \"words.txt\". Make sure it is in the same directory as the \"Hangman\" executable.\n";
    return false;
  }

  // Read File
  while (std::getline(file, word)) 
  {
    if(word.find("Easy") != std::string::npos)
    {
      difficulty = EASY;
      std::cout << "Reading \"Easy\" words...\n";
      continue;
    }
    else if(word.find("Medium") != std::string::npos) 
    {
      difficulty = MEDIUM;
      std::cout << "Reading \"Medium\" words...\n";
      continue;
    }
    else if(word.find("Hard") != std::string::npos)
    {
      difficulty = HARD;
      std::cout << "Reading \"Hard\" words...\n";
      continue;
    }
    else if(word.find("Dictionary") != std::string::npos)
    {
      difficulty = DICTIONARY;
      std::cout << "Reading \"Dictionary\" words...\n";
      continue;
    }
    words[static_cast<unsigned short>(difficulty)].push_back(word);
  }

  std::cout << "Done reading file.\n\n";
  return true;
}

Difficulty ChooseDifficulty()
{
  unsigned short choice;
  Difficulty difficulty; 
  bool chosen = false;

  std::cout << "1 - Easy\n2 - Medium\n3 - Hard\n4 - Dictionary\n";
  while(!chosen)
  {
    std::cout << "Choose a difficulty: ";
    std::cin >> choice;

    chosen = true;
    difficulty = static_cast<Difficulty>(choice); 
    if(difficulty == EASY)
    {
      std::cout << "Your chosen Diffuculty is Easy\n";
    }
    else if(difficulty == MEDIUM)
    {
      std::cout << "Your chosen Diffuculty is Medium\n";
    }
    else if(difficulty == HARD)
    {
      std::cout << "Your chosen Diffuculty is Hard\n";
    }
    else if(difficulty == DICTIONARY)
    {
      std::cout << "Your chosen Diffuculty is Dictionary\n";
    }
    else 
    {
      std::cout << "Invalid choice. Try again\n";
      std::cin.clear();
      std::cin.ignore();
      chosen = false;
    }
  }
  std::cout << "\n----------------------------------------------\n\n";
  return difficulty;
}

void BeginGame(
  std::unordered_map<unsigned short, std::vector<std::string>>& wordsMap,
  Difficulty difficulty
)
{
  std::vector<std::string> words = wordsMap[difficulty];
  int maxTries;
  
  switch (difficulty) 
  {
  case EASY:
    maxTries = 6;
    break;
  case MEDIUM:
    maxTries = 9;
    break;
  case HARD:
    maxTries = 10;
    break;
  case DICTIONARY:
    maxTries = 12;
    break;
  default:
    maxTries = 0;
    break;
  }

  std::string showString = "";
  std::string hangWord;
  std::string guess;
  int length;

  bool solved = false;

  ChooseWord(words, hangWord);
  length = hangWord.size();
  for(int i = 0; i < hangWord.size(); ++i)
  {
    showString += "_";
  }
  
  while(!solved && maxTries > 0)
  {
    std::cout << "You have " << maxTries << " tries left.\n";
    std::cout << length << " characters left to guess.\n";
    std::cout << showString << "\n";
    std::cout << "Take your guess: ";
    std::cin >> guess;   
    solved = CheckWord(guess, showString, hangWord, maxTries, length);
  }

  if(!solved)
  {
    std::cout << "The word was \"" << hangWord << "\".\n\n";
  }
  else 
  {
    std::cout << "Yay! You got the word \"" << hangWord << "\"!\n\n";
  }
}

