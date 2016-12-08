#pragma warning(disable: 4996)

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <boost/algorithm/string.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/adaptor/strided.hpp>
#include "InputUtils.h"

using namespace std;

bool supportsTls(string ip);
bool isAbba(string text);

string day7Solution()
{
    ifstream inputFile;
    inputFile.open("day7_input.txt");

    vector<string> lines = InputUtils::readLines(inputFile);
    auto numSupportTls = count_if(lines.begin(), lines.end(), supportsTls);

    return to_string(numSupportTls);
}

bool supportsTls(string ip)
{
    // Ip and hypernet parts are split by the '[' and ']' surrounding the hypernet part
    vector<string> addressParts;
    boost::split(addressParts, ip, boost::is_any_of("[]"));

    // The parts will alternate between an ip part and hypernet part, create iterators to these parts
    auto ipPartIterator = addressParts | boost::adaptors::strided(2);
    auto hypernetPartIterator = boost::make_iterator_range(addressParts.begin() + 1, addressParts.end())
        | boost::adaptors::strided(2);

    bool anyIpPartIsAbba = any_of(ipPartIterator.begin(), ipPartIterator.end(), isAbba);
    bool anyHypernetPartIsAbba = any_of(hypernetPartIterator.begin(), hypernetPartIterator.end(), isAbba);

    return anyIpPartIsAbba && !anyHypernetPartIsAbba;
}

bool isAbba(string text)
{
    bool isAbba = false;
    for (unsigned int i = 0; i < text.size() - 3 && !isAbba; i++)
    {
        isAbba |= text[i] == text[i + 3] && text[i + 1] == text[i + 2] && text[i] != text[i + 1];
    }

    return isAbba;
}