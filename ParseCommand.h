#pragma once
#include <string.h>
#include <locale>
#include <iostream>

bool matchString(std::string line, int& index, char* str);
bool getNumbers(std::string line, int& index, long int& min, long int& max);
