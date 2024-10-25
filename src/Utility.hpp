#pragma once
#include "Difficulty.hpp"
#include <string>
#include <unordered_map>
#include <vector>

Difficulty ChooseDifficulty();
bool LoadWords(std::unordered_map<Difficulty, std::vector<std::string>> &words);
