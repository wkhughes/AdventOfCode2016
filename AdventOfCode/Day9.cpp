#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <regex>

using namespace std;

string day9Solution()
{
    ifstream inputFile("day9_input.txt");
    string compressedText{istreambuf_iterator<char>(inputFile), istreambuf_iterator<char>()};
    compressedText.erase(remove_if(compressedText.begin(), compressedText.end(), isspace));

    auto nextCharacter = compressedText.cbegin();
    unsigned int decompressedFileLength = 0;

    regex markerRegex(R"(\((\d+)x(\d+)\))");
    smatch markerMatch;
    while (regex_search(nextCharacter, compressedText.cend(), markerMatch, markerRegex))
    {
        // Increment file length by number of characters read up to the marker
        decompressedFileLength += markerMatch.position();

        // Decompressed length is incremented by the number of repeated characters that the marker indicates
        int numCharacters = stoi(markerMatch[1]);
        int repetitions = stoi(markerMatch[2]);
        decompressedFileLength += numCharacters * repetitions;

        // Look for the next marker from past the repeated characters
        nextCharacter += markerMatch.position() + markerMatch.length() + numCharacters;
    }

    // No more markers exist, the remaining length is to the end of the text
    decompressedFileLength += distance(nextCharacter, compressedText.cend());
    return to_string(decompressedFileLength);
}