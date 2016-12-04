#include <vector>
#include <iostream>
#include <string>
#include "InputUtils.h"

using namespace std;

namespace InputUtils
{
    vector<string> readLines(istream& input)
    {
        return split(input, '\n');
    }

    vector<string> split(istream& input, char delimiter)
    {
        vector<string> parts;

        string part;
        while (getline(input, part, delimiter))
        {
            parts.push_back(part);
        }

        return parts;
    }
}