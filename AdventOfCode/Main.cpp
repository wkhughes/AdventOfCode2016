#include <iostream>
#include <string>
#include <map>
#include <functional>
#include "Day1.h"
#include "Day2.h"
#include "Day3.h"

using namespace std;

const int DEFAULT_DAY = 3;
const map<int, function<string ()>> DAY_SOLUTIONS
{
    { 1, day1Solution },
    { 2, day2Solution },
    { 3, day3Solution },
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