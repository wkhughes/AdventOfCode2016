#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

struct TriangleSides {
    int a;
    int b;
    int c;

    int perimiter() const {
        return a + b + c;
    }

    int longestSide() const {
        return max({ a, b, c });
    }
};

vector<TriangleSides> readSides(istream& input);
TriangleSides getSidesFromText(const string& sidesText);
bool isPossibleTriangle(const TriangleSides& sides);

int main() {
    ifstream inputFile;
    inputFile.open("day3_input.txt");

    vector<TriangleSides> sides = readSides(inputFile);
    auto numPossible = count_if(sides.begin(), sides.end(), isPossibleTriangle);

    cout << numPossible << endl;

    return 0;
}

bool isPossibleTriangle(const TriangleSides& sides) {
    return sides.longestSide() < sides.perimiter() - sides.longestSide();
}

vector<TriangleSides> readSides(istream& input) {
    vector<string> lines;
    std::string line;
    while (getline(input, line)) {
        lines.push_back(line);
    }

    vector<TriangleSides> allSides;
    allSides.reserve(lines.size());
    transform(lines.begin(), lines.end(), back_inserter(allSides), getSidesFromText);

    return allSides;
}

TriangleSides getSidesFromText(const string& sidesText) {
    TriangleSides sides;
    istringstream iss(sidesText);

    iss >> sides.a;
    iss >> sides.b;
    iss >> sides.c;

    return sides;
}