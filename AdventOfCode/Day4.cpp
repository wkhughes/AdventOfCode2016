#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <queue>
#include <regex>
#include "InputUtils.h"

using namespace std;

constexpr int ALPHABET_SIZE = 26;

struct CharacterFrequency
{
    char character;
    int frequency;

    bool operator<(const CharacterFrequency& other) const
    {
        return frequency < other.frequency || (frequency == other.frequency && character > other.character);
    }
};

string day4Solution()
{
    int realSectorIdSum = 0;

    ifstream inputFile("day4_input.txt");

    vector<string> lines = InputUtils::readLines(inputFile);
    for (string& line : lines)
    {
        // Remove the dashes from the encrypted room name
        line.erase(remove(line.begin(), line.end(), '-'), line.end());

        regex roomRegex("^([a-z]+)(\\d+)\\[([a-z]+)\\]$");
        smatch roomRegexMatches;
        if (regex_search(line, roomRegexMatches, roomRegex))
        {
            string name = roomRegexMatches[1];
            int sectorId = stoi(roomRegexMatches[2]);
            string checksum = roomRegexMatches[3];

            int frequencies[ALPHABET_SIZE] = { 0 };
            for (const char& character : name)
            {
                frequencies[character - 'a']++;
            }

            priority_queue<CharacterFrequency> orderedFrequencies;
            for (char character = 'a'; character <= 'z'; character++)
            {
                orderedFrequencies.push({ character, frequencies[character - 'a'] });
            }

            bool isRealRoom = true;
            for (const char& checksumCharacter : checksum)
            {
                isRealRoom &= checksumCharacter == orderedFrequencies.top().character;
                orderedFrequencies.pop();
            }

            if (isRealRoom)
            {
                realSectorIdSum += sectorId;
            }
        }
    }

    return to_string(realSectorIdSum);
}