#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <iterator>
#include <array>
#include <numeric>
#include <memory>
#include <intrin.h>

using namespace std;

constexpr unsigned int FAVORITE_NUMBER = 1350;

struct Space
{
    int x;
    int y;

    bool isOpen() const
    {
        unsigned int sum = x * x + 3 * x + 2 * x * y + y + y * y;
        sum += FAVORITE_NUMBER;
        return __popcnt(sum) % 2 == 0;
    }

    vector<Space> getNeighbourSpaces() const
    {
        vector<Space> candidateSpaces =
        {
            { x - 1, y },
            { x + 1, y },
            { x, y + 1 },
            { x, y - 1 }
        };

        candidateSpaces.erase(remove_if(candidateSpaces.begin(), candidateSpaces.end(), [](const auto& space)
        {
            return space.x < 0 || space.y < 0 || !space.isOpen();
        }), candidateSpaces.end());

        return candidateSpaces;
    }
};

bool operator<(const Space& left, const Space& right)
{
    return left.x < right.x || (left.x == right.x && left.y < right.y);
}

unsigned int calculateStepsBetweenSpaces(const Space& startSpace, const Space& endSpace);

string day13Solution()
{
    return to_string(calculateStepsBetweenSpaces({ 1, 1 }, { 31, 39 }));
}

unsigned int calculateStepsBetweenSpaces(const Space& startSpace, const Space& endSpace)
{
    unique_ptr<Space> completeSpace = nullptr;
    queue<Space> nextSpaces;
    map<Space, unsigned int> stepsFromStartSpace;

    nextSpaces.push(startSpace);

    while (!nextSpaces.empty() && completeSpace == nullptr)
    {
        auto currentSpace = nextSpaces.front();
        nextSpaces.pop();

        if (currentSpace.x == endSpace.x && currentSpace.y == endSpace.y)
        {
            completeSpace = make_unique<Space>(currentSpace);
        }
        else
        {
            for (const auto& nextSpace : currentSpace.getNeighbourSpaces())
            {
                bool isVisitedSpace = stepsFromStartSpace.count(nextSpace) != 0;
                unsigned int stepsFromStart = stepsFromStartSpace[currentSpace] + 1;
                if (!isVisitedSpace || stepsFromStart < stepsFromStartSpace[nextSpace])
                {
                    stepsFromStartSpace[nextSpace] = stepsFromStart;
                    nextSpaces.push(nextSpace);
                }
            }
        }
    }

    if (completeSpace != nullptr)
    {
        return stepsFromStartSpace[*completeSpace];
    }
    else
    {
        throw invalid_argument("Cannot find path to complete space from given start space");
    }
}