#pragma once

#include <vector>
#include <iostream>
#include <string>

namespace InputUtils
{
    std::vector<std::string> readLines(std::istream& input);
    std::vector<std::string> split(std::istream& input, char delimiter);
}