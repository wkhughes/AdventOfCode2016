#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include "Day1.h"
#include "Day2.h"
#include "Day3.h"
#include "Day4.h"
#include "Day5.h"
#include "Day6.h"
#include "Day7.h"
#include "Day8.h"
#include "Day9.h"
#include "Day10.h"
#include "Day11.h"

using namespace std;

constexpr int DEFAULT_DAY = 11;
const unordered_map<int, function<string ()>> DAY_SOLUTIONS
{
    { 1, day1Solution },
    { 2, day2Solution },
    { 3, day3Solution },
    { 4, day4Solution },
    { 5, day5Solution },
    { 6, day6Solution },
    { 7, day7Solution },
    { 8, day8Solution },
    { 9, day9Solution },
    { 10, day10Solution },
    { 11, day11Solution }
};

int main(int argc, char* argv[])
{
    int day = argc >= 2 ? stoi(argv[1]) : DEFAULT_DAY;
    
    auto solution = DAY_SOLUTIONS.find(day);
    if (solution != DAY_SOLUTIONS.end())
    {
        try
        {
            cout << solution->second() << endl;
        }
        catch (const exception& e)
        {
            cerr << "Failure running solution for day " << day << " due to the following exception:\n" << e.what() << endl;
        }
    }
    else
    {
        cerr << "No solution exists for day " << day << endl;
    }
}