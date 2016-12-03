#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

enum class Direction
{
    North,
    East,
    South,
    West
};

Direction goRight(Direction direction);
Direction goLeft(Direction direction);

string instructions = "R4, R1, L2, R1, L1, L1, R1, L5, R1, R5, L2, R3, L3, L4, R4, R4, R3, L5, L1, R5, R3, L4, R1, R5, L1, R3, L2, R3, R1, L4, L1, R1, L1, L5, R1, L2, R2, L3, L5, R1, R5, L1, R188, L3, R2, R52, R5, L3, R79, L1, R5, R186, R2, R1, L3, L5, L2, R2, R4, R5, R5, L5, L4, R5, R3, L4, R4, L4, L4, R5, L4, L3, L1, L4, R1, R2, L5, R3, L4, R3, L3, L5, R1, R1, L3, R2, R1, R2, R2, L4, R5, R1, R3, R2, L2, L2, L1, R2, L1, L3, R5, R1, R4, R5, R2, R2, R4, R4, R1, L3, R4, L2, R2, R1, R3, L5, R5, R2, R5, L1, R2, R4, L1, R5, L3, L3, R1, L4, R2, L2, R1, L1, R4, R3, L2, L3, R3, L2, R1, L4, R5, L1, R5, L2, L1, L5, L2, L5, L2, L4, L2, R3";

string day1Solution()
{
    auto direction = Direction::North;
    int verticalSteps = 0;
    int horizontalSteps = 0;

    instructions.erase(remove(instructions.begin(), instructions.end(), ' '), instructions.end());

    istringstream iss(instructions);
    string instruction;
    while (getline(iss, instruction, ','))
    {
        direction = instruction[0] == 'L' ? goLeft(direction) : goRight(direction);
        int steps = stoi(instruction.substr(1, instruction.length() - 1));

        if (direction == Direction::North)
        {
            verticalSteps += steps;
        }
        else if (direction == Direction::South)
        {
            verticalSteps -= steps;
        }
        else if (direction == Direction::East)
        {
            horizontalSteps += steps;
        }
        else
        {
            horizontalSteps -= steps;
        }
    }

    return to_string(abs(verticalSteps) + abs(horizontalSteps));
}

Direction goRight(Direction direction)
{
    if (direction == Direction::North) return Direction::East;
    if (direction == Direction::East) return Direction::South;
    if (direction == Direction::South) return Direction::West;
    return Direction::North;
}

Direction goLeft(Direction direction)
{
    if (direction == Direction::North) return Direction::West;
    if (direction == Direction::West) return Direction::South;
    if (direction == Direction::South) return Direction::East;
    return Direction::North;
}