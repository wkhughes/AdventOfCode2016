#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <iterator>
#include "InputUtils.h"

using namespace std;

constexpr int ALPHABET_SIZE = 26;

string day6Solution()
{
    ifstream inputFile;
    inputFile.open("day6_input.txt");

    vector<string> lines = InputUtils::readLines(inputFile);
    vector<array<int, ALPHABET_SIZE>> frequencies(lines[0].length(), { 0 });
    vector<char> mostCommonCharacter(lines[0].length(), 'a');

    for (string& line : lines)
    {
        for (unsigned int i = 0; i < line.length(); i++)
        {
            int& characterFrequencyForColumn = frequencies[i][line[i] - 'a'];
            int mostCommonCharacterFrequencyForColumn = frequencies[i][mostCommonCharacter[i] - 'a'];

            characterFrequencyForColumn++;

            if (characterFrequencyForColumn > mostCommonCharacterFrequencyForColumn)
            {
                mostCommonCharacter[i] = line[i];
            }
        }
    }

    return { mostCommonCharacter.begin(), mostCommonCharacter.end() };
}