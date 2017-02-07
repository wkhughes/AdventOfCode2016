#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <limits>
#include <stdexcept>

using namespace std;

struct Disc
{
    unsigned int numPositions;
    unsigned int startPosition;

    bool isOpenAtTime(unsigned int time) const
    {
        return (startPosition + time) % numPositions == 0;
    }
};

const vector<Disc> SCULPTURE_DISCS =
{
    { 13, 11 },
    { 5, 0 },
    { 17, 11 },
    { 3, 0 },
    { 7, 2 },
    { 19, 17 }
};

bool discsOpenAtTime(const vector<Disc>& discs, unsigned int time);

string day15Solution()
{
    unsigned int time;
    bool opened = false;
    for (time = 0; time < numeric_limits<unsigned int>::max() && !opened; time++)
    {
        if (discsOpenAtTime(SCULPTURE_DISCS, time + 1))
        {
            return to_string(time);
        }
    }

    throw invalid_argument("Cannot find any open time for given discs");
}

bool discsOpenAtTime(const vector<Disc>& discs, unsigned int time)
{
    if (discs.empty())
    {
        return false;
    }
    else if (discs.size() == 1)
    {
        return discs[0].isOpenAtTime(time);
    }
    else
    {
        return discs[0].isOpenAtTime(time) && discsOpenAtTime(vector<Disc>(discs.begin() + 1, discs.end()), time + 1);
    }
}