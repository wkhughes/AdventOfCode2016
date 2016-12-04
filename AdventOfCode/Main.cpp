#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include "Day1.h"
#include "Day2.h"
#include "Day3.h"
#include "Day4.h"

using namespace std;

constexpr int DEFAULT_DAY = 4;
const unordered_map<int, function<string ()>> DAY_SOLUTIONS
{
    { 1, day1Solution },
    { 2, day2Solution },
    { 3, day3Solution },
    { 4, day4Solution }
};

int main(int argc, char* argv[])
{
    int day = argc >= 2 ? stoi(argv[1]) : DEFAULT_DAY;
    
    auto solution = DAY_SOLUTIONS.find(day);
    if (solution != DAY_SOLUTIONS.end())
    {
        cout << solution->second() << endl;
    }
    else
    {
        cerr << "No solution exists for day " << day << endl;
    }
}