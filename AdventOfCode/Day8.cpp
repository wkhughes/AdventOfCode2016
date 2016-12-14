#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <valarray>
#include <memory>
#include <stdexcept>
#include <regex>
#include "InputUtils.h"

using namespace std;

using PixelMatrix = valarray<bool>;

constexpr unsigned int SCREEN_WIDTH = 50;
constexpr unsigned int SCREEN_HEIGHT = 6;

const regex RECT_REGEX(R"(rect (\d+)x(\d+))");
const regex ROTATE_ROW_REGEX(R"(rotate row y=(\d+) by (\d+))");
const regex ROTATE_COLUMN_REGEX(R"(rotate column x=(\d+) by (\d+))");

struct Operation
{
    virtual void apply(PixelMatrix& pixels) = 0;
};

struct RectOperation : public Operation
{
    unsigned int width;
    unsigned int height;

    void apply(PixelMatrix& pixels)
    {
        // Slice the first column up to rect height, then across the rows up to the rect width, and set the pixels on
        auto rectSlice = gslice(0, { height, width }, { SCREEN_WIDTH, 1 });
        pixels[rectSlice] = true;
    }
};

struct RotateRowOperation : public Operation
{
    unsigned int y;
    int amount;

    void apply(PixelMatrix& pixels)
    {
        // Slice the entire y row and rotate (circular shift) right
        auto rowSlice = slice(SCREEN_WIDTH * y, SCREEN_WIDTH, 1);
        pixels[rowSlice] = PixelMatrix(pixels[rowSlice]).cshift(-amount);
    }
};

struct RotateColumnOperation : public Operation
{
    unsigned int x;
    int amount;

    void apply(PixelMatrix& pixels)
    {
        // Slice the entire x column and rotate (circular shift) right, equivilent to rotating the column down
        auto columnSlice = slice(x, SCREEN_HEIGHT, SCREEN_WIDTH);
        pixels[columnSlice] = PixelMatrix(pixels[columnSlice]).cshift(-amount);
    }
};

unique_ptr<Operation> parseOperation(string text);

string day8Solution()
{
    ifstream inputFile;
    inputFile.open("day8_input.txt");
    auto lines = InputUtils::readLines(inputFile);

    vector<unique_ptr<Operation>> operations;
    operations.resize(lines.size());
    transform(lines.begin(), lines.end(), operations.begin(), parseOperation);

    PixelMatrix pixels(SCREEN_WIDTH * SCREEN_HEIGHT);
    for_each(operations.begin(), operations.end(), [&pixels](const auto& operation)
    {
        operation->apply(pixels);
    });

    return to_string(count(begin(pixels), end(pixels), true));
}

unique_ptr<Operation> parseOperation(string text)
{
    unique_ptr<Operation> operation;

    smatch regexMatches;
    if (regex_search(text, regexMatches, RECT_REGEX))
    {
        RectOperation rect;
        rect.width = stoi(regexMatches[1]);
        rect.height = stoi(regexMatches[2]);
        operation = make_unique<RectOperation>(rect);
    }
    else if (regex_search(text, regexMatches, ROTATE_ROW_REGEX))
    {
        RotateRowOperation rotateRow;
        rotateRow.y = stoi(regexMatches[1]);
        rotateRow.amount = stoi(regexMatches[2]);
        operation = make_unique<RotateRowOperation>(rotateRow);
    }
    else if (regex_search(text, regexMatches, ROTATE_COLUMN_REGEX))
    {
        RotateColumnOperation rotateColumn;
        rotateColumn.x = stoi(regexMatches[1]);
        rotateColumn.amount = stoi(regexMatches[2]);
        operation = make_unique<RotateColumnOperation>(rotateColumn);
    }
    else
    {
        throw invalid_argument("Cannot parse operation from text '" + text + "'");
    }

    return operation;
}