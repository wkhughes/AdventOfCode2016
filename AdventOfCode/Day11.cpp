#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <iterator>
#include <array>
#include <numeric>
#include <intrin.h>
#include <boost/functional/hash.hpp>

using namespace std;

constexpr unsigned int FLOOR_COUNT = 4;
constexpr unsigned int ELEMENT_COUNT = 5;
constexpr unsigned int OBJECT_COUNT = ELEMENT_COUNT * 2;
constexpr unsigned int ELEVATOR_CAPACITY = 2;

struct State
{
    unsigned int elevatorFloor;
    array<unsigned int, OBJECT_COUNT> objectFloors;

    vector<unsigned int> getMicrochipFloors() const
    {
        return { objectFloors.begin(), objectFloors.begin() + ELEMENT_COUNT };
    }

    vector<unsigned int> getGeneratorFloors() const
    {
        return { objectFloors.begin() + ELEMENT_COUNT, objectFloors.end() };
    }

    unsigned int numObjectsOnFloor(unsigned int floor) const
    {
        return count(objectFloors.begin(), objectFloors.end(), floor);
    }

    auto getObjectsOnFloor(unsigned int floor) -> decltype(objectFloors.begin()) const
    {
        return find(objectFloors.begin(), objectFloors.end(), floor);
    }

    bool isValid() const
    {
        auto microchipFloors = getMicrochipFloors();
        auto generatorFloors = getGeneratorFloors();

        for (unsigned int i = 0; i < ELEMENT_COUNT; i++)
        {
            if (microchipFloors[i] != generatorFloors[i]
                && find(generatorFloors.begin(), generatorFloors.end(), microchipFloors[i]) != generatorFloors.end())
            {
                return false;
            }
        }

        return true;
    }

    bool isCompleteState() const
    {
        return numObjectsOnFloor(FLOOR_COUNT - 1) == OBJECT_COUNT;
    }
};

const State INITIAL_STATE
{
    0,
    {
        0, 0, 2, 1, 1,
        0, 0, 1, 1, 1
    }
};

namespace std
{
    template<>
    struct hash<State>
    {
        size_t operator()(const State& state) const noexcept
        {
            auto microchipFloors = state.getMicrochipFloors();
            auto generatorFloors = state.getGeneratorFloors();

            vector<unsigned int> microchipGeneratorPairHashes;

            for (unsigned int i = 0; i < ELEMENT_COUNT; i++)
            {
                size_t pairHash = 0;
                boost::hash_combine(pairHash, microchipFloors[i]);
                boost::hash_combine(pairHash, generatorFloors[i]);
                microchipGeneratorPairHashes.push_back(pairHash);
            }

            // Sort the microchip/generator pairs so the floor combination of one element is equivalent to another
            // element pair on the same floors
            sort(microchipGeneratorPairHashes.begin(), microchipGeneratorPairHashes.end());

            size_t seed = 0;
            boost::hash_combine(seed, state.elevatorFloor);
            boost::hash_range(seed, microchipGeneratorPairHashes.begin(), microchipGeneratorPairHashes.end());

            return seed;
        }
    };
}

bool operator==(const State& left, const State& right)
{
    return hash<State>()(left) == hash<State>()(right);
}

bool operator!=(const State& left, const State& right)
{
    return !(left == right);
}

unsigned int calculateStepsToCompleteState(const State& initialState);
vector<State> findValidNeighborStates(const State& state);
vector<State> findPossibleNeighborStates(State state);
vector<unsigned int> getFloorsAdjacentToFloor(unsigned int floor);

string day11Solution()
{
    return to_string(calculateStepsToCompleteState(INITIAL_STATE));
}

unsigned int calculateStepsToCompleteState(const State& initialState)
{
    unique_ptr<State> completeState = nullptr;
    queue<State> nextStates;
    unordered_map<State, unsigned int> stepsFromInitialState;

    nextStates.push(initialState);

    while (!nextStates.empty() && completeState == nullptr)
    {
        auto currentState = nextStates.front();
        nextStates.pop();

        if (currentState.isCompleteState())
        {
            completeState = make_unique<State>(currentState);
        }
        else
        {
            for (const auto& nextState : findValidNeighborStates(currentState))
            {
                bool isVisitedState = stepsFromInitialState.count(nextState) != 0;
                unsigned int stateSteps = stepsFromInitialState[currentState] + 1;
                if (!isVisitedState || stateSteps < stepsFromInitialState[nextState])
                {
                    stepsFromInitialState[nextState] = stateSteps;
                    nextStates.push(nextState);
                }
            }
        }
    }

    if (completeState != nullptr)
    {
        return stepsFromInitialState[*completeState];
    }
    else
    {
        throw invalid_argument("Cannot find path to complete state from given initial state");
    }
}

vector<State> findValidNeighborStates(const State& state)
{
    auto& states = findPossibleNeighborStates(state);

    states.erase(remove_if(states.begin(), states.end(), [](const auto& neighbor)
    {
        return !neighbor.isValid();
    }), states.end());

    return states;
}

vector<State> findPossibleNeighborStates(State state)
{
    vector<State> neighborStates;

    for (unsigned int objectsMask = 1; objectsMask < (1u << state.numObjectsOnFloor(state.elevatorFloor)); objectsMask++)
    {
        if (__popcnt(objectsMask) <= ELEVATOR_CAPACITY)
        {
            for (unsigned int floor : getFloorsAdjacentToFloor(state.elevatorFloor))
            {
                State newState = state;
                newState.elevatorFloor = floor;

                for (unsigned int i = 0, floorObjectCount = 0; i < OBJECT_COUNT; i++)
                {
                    if (state.objectFloors[i] == state.elevatorFloor)
                    {
                        // If the floor object has been selected by the mask, move it to the new floor
                        if ((objectsMask >> floorObjectCount) & 1)
                        {
                            newState.objectFloors[i] = floor;
                        }

                        floorObjectCount++;
                    }
                }

                neighborStates.push_back(newState);
            }
        }
    }

    return neighborStates;
}

vector<unsigned int> getFloorsAdjacentToFloor(unsigned int floor)
{
    vector<unsigned int> floors;

    if (floor > 0)
    {
        floors.push_back(floor - 1);
    }
    if (floor < FLOOR_COUNT - 1)
    {
        floors.push_back(floor + 1);
    }

    return floors;
}