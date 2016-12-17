#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <regex>
#include <list>
#include <unordered_map>
#include <deque>
#include "InputUtils.h"

using namespace std;

enum class DestinationType
{
    Bot,
    Output
};

struct Microchips
{
    unsigned int numMicrochips = 0;
    unsigned int lowValue;
    unsigned int highValue;

    void giveMicrochip(unsigned int value)
    {
        if (numMicrochips > 0)
        {
            lowValue = min(lowValue, value);
            highValue = max(highValue, value);
            numMicrochips++;
        }
        else
        {
            lowValue = value;
            highValue = value;
            numMicrochips++;
        }
    }
};

struct MicrochipDestination
{
    DestinationType destinationType;
    unsigned int number;
};

struct BotLogic
{
    MicrochipDestination lowDestination;
    MicrochipDestination highDestination;
};

using BotMicrochipsMap = unordered_map<unsigned int, Microchips>;
using BotLogicMap = unordered_map<unsigned int, BotLogic>;

DestinationType parseDestinationType(string text);
void parseBotInstruction(string text, BotMicrochipsMap& botMicrochips, BotLogicMap& botLogic);
unsigned int findBotWithMicrochipValues(BotMicrochipsMap& botMicrochips, BotLogicMap& botLogic, unsigned int lowValue, unsigned int highValue);

constexpr unsigned int TARGET_LOW_VALUE = 17;
constexpr unsigned int TARGET_HIGH_VALUE = 61;

const regex VALUE_TO_BOT_REGEX(R"(^value (\d+) goes to bot (\d+)$)");
const regex LOW_AND_HIGH_VALUE_REGEX(R"(^bot (\d+) gives low to (bot|output) (\d+) and high to (bot|output) (\d+)$)");

string day10Solution()
{
    ifstream inputFile("day10_input.txt");
    vector<string> lines = InputUtils::readLines(inputFile);

    BotMicrochipsMap botMicrochips;
    BotLogicMap botLogic;

    for_each(lines.begin(), lines.end(), [&botMicrochips, &botLogic](const auto& line)
    {
        parseBotInstruction(line, botMicrochips, botLogic);
    });

    unsigned int foundBotNumber = findBotWithMicrochipValues(botMicrochips, botLogic, TARGET_LOW_VALUE, TARGET_HIGH_VALUE);
    return to_string(foundBotNumber);
}

void parseBotInstruction(string text, BotMicrochipsMap& botMicrochips, BotLogicMap& botLogic)
{
    smatch regexMatches;
    if (regex_match(text, regexMatches, VALUE_TO_BOT_REGEX))
    {
        unsigned int value = stoi(regexMatches[1]);
        unsigned int botNumber = stoi(regexMatches[2]);

        if (botMicrochips.count(botNumber) == 0)
        {
            botMicrochips[botNumber] = {};
        }

        botMicrochips[botNumber].giveMicrochip(value);
    }
    else if (regex_match(text, regexMatches, LOW_AND_HIGH_VALUE_REGEX))
    {
        unsigned int botNumber = stoi(regexMatches[1]);
        MicrochipDestination lowDestination =
        {
            parseDestinationType(regexMatches[2]),
            static_cast<unsigned int>(stoi(regexMatches[3]))
        };
        MicrochipDestination highDestination =
        {
            parseDestinationType(regexMatches[4]),
            static_cast<unsigned int>(stoi(regexMatches[5]))
        };
        botLogic[botNumber] = { lowDestination, highDestination };
    }
    else
    {
        throw invalid_argument("Cannot parse instruction from text '" + text + "'");
    }
}

DestinationType parseDestinationType(string text)
{
    DestinationType destinationType;

    if (text == "bot")
    {
        destinationType = DestinationType::Bot;
    }
    else if (text == "output")
    {
        destinationType = DestinationType::Output;
    }
    else
    {
        throw invalid_argument("Cannot parse destination type from text '" + text + "'");
    }

    return destinationType;
}


unsigned int findBotWithMicrochipValues(BotMicrochipsMap& botMicrochips, BotLogicMap& botLogic, unsigned int lowValue, unsigned int highValue)
{
    unsigned int foundBotNumber;

    auto initialBotWithTwoMicrochips = find_if(botMicrochips.begin(), botMicrochips.end(), [](const auto& entry)
    {
        return entry.second.numMicrochips == 2;
    });

    deque<unsigned int> botsWithTwoMicrochips;
    botsWithTwoMicrochips.push_back(initialBotWithTwoMicrochips->first);

    while (!botsWithTwoMicrochips.empty())
    {
        unsigned int botNumber = botsWithTwoMicrochips.back();
        botsWithTwoMicrochips.pop_back();

        const auto& currentBotMicrochips = botMicrochips[botNumber];
        if (currentBotMicrochips.lowValue == lowValue && currentBotMicrochips.highValue == highValue)
        {
            foundBotNumber = botNumber;
        }
        else
        {
            const auto& lowDestination = botLogic[botNumber].lowDestination;
            const auto& highDestination = botLogic[botNumber].highDestination;

            if (lowDestination.destinationType == DestinationType::Bot)
            {
                botMicrochips[lowDestination.number].giveMicrochip(currentBotMicrochips.lowValue);
                if (botMicrochips[lowDestination.number].numMicrochips == 2)
                {
                    botsWithTwoMicrochips.push_back(lowDestination.number);
                }
            }

            if (highDestination.destinationType == DestinationType::Bot)
            {
                botMicrochips[highDestination.number].giveMicrochip(currentBotMicrochips.highValue);
                if (botMicrochips[highDestination.number].numMicrochips == 2)
                {
                    botsWithTwoMicrochips.push_back(highDestination.number);
                }
            }
        }
    }

    return foundBotNumber;
}