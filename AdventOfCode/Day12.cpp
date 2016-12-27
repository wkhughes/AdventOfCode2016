#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <boost/algorithm/string.hpp>
#include "InputUtils.h"

using namespace std;

struct Registers
{
    int a;
    int b;
    int c;
    int d;
};

struct Cpu
{
    Registers registers = {0};
    unsigned int instructionPointer = 0;
};

struct Register
{
    int Registers::*accessor;

    Register(int Registers::*accessor)
        : accessor(accessor)
    {
    }

    int read(Cpu& cpu)
    {
        return cpu.registers.*accessor;
    }

    void write(Cpu& cpu, int value)
    {
        cpu.registers.*accessor = value;
    }
};

void executeInstruction(string instruction, Cpu& cpu);
vector<string> tokenizeInstruction(string instruction);
int readImmediateOrRegisterValue(Cpu& cpu, string operand);
bool isInteger(string text);
Register createRegisterForLetter(string letter);

string day12Solution()
{
    ifstream inputFile("day12_input.txt");
    auto instructions = InputUtils::readLines(inputFile);

    Cpu cpu;
    while (cpu.instructionPointer < instructions.size())
    {
        executeInstruction(instructions[cpu.instructionPointer], cpu);
    }

    return to_string(cpu.registers.a);
}

void executeInstruction(string instruction, Cpu& cpu)
{
    auto tokens = tokenizeInstruction(instruction);
    auto operation = tokens[0];
    vector<string> operands(tokens.begin() + 1, tokens.end());

    unsigned int numOperandsRead = 0;

    if (operation == "cpy")
    {
        int source = readImmediateOrRegisterValue(cpu, operands[numOperandsRead++]);
        createRegisterForLetter(operands[numOperandsRead++]).write(cpu, source);

        cpu.instructionPointer++;
    }
    else if (operation == "inc")
    {
        Register reg = createRegisterForLetter(operands[numOperandsRead++]);
        int currentValue = reg.read(cpu);
        reg.write(cpu, currentValue + 1);

        cpu.instructionPointer++;
    }
    else if (operation == "dec")
    {
        Register reg = createRegisterForLetter(operands[numOperandsRead++]);
        int currentValue = reg.read(cpu);
        reg.write(cpu, currentValue - 1);

        cpu.instructionPointer++;
    }
    else if (operation == "jnz")
    {
        int value = readImmediateOrRegisterValue(cpu, operands[numOperandsRead++]);
        if (value != 0)
        {
            cpu.instructionPointer += stoi(operands[numOperandsRead++]);
        }
        else
        {
            cpu.instructionPointer++;
        }
    }
}

vector<string> tokenizeInstruction(string instruction)
{
    vector<string> tokens;
    boost::split(tokens, instruction, isspace);
    return tokens;
}

int readImmediateOrRegisterValue(Cpu& cpu, string operand)
{
    int value;
    if (isInteger(operand))
    {
        value = stoi(operand);
    }
    else
    {
        value = createRegisterForLetter(operand).read(cpu);
    }

    return value;
}

bool isInteger(string text)
{
    return text[0] == '-' || isdigit(text[0]);
}

Register createRegisterForLetter(string letter)
{
    switch (letter[0])
    {
        case 'a':
            return Register(&Registers::a);
        case 'b':
            return Register(&Registers::b);
        case 'c':
            return Register(&Registers::c);
        case 'd':
            return Register(&Registers::d);
        default:
            throw invalid_argument("Unknown register letter " + letter);
    }
}