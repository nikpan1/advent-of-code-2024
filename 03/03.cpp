//
// Created by nikod on 01/12/2024.
//

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

std::vector<std::string> loadInput(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<std::string> result;
    std::string line;
    while (std::getline(file, line))
    {
        result.push_back(line);
    }

    return result;
}

// -------------------------------- First Part ------------------------------------------------

struct MulNums
{
    MulNums(int _num1, int _num2, size_t _commaIndex)
        : num1(_num1), num2(_num2), commaIndex(_commaIndex) {}

    int num1;
    int num2;
    size_t commaIndex;
};

const std::string MUL_EXPR ="mul(";

size_t getEndIndexOfNumber(const std::string& filename, int i)
{
    while (i < filename.size() && std::isdigit(filename[i])) i ++;
    return i;
}

MulNums loadFirstNum(const std::string& filename, int index) {
    const size_t i = getEndIndexOfNumber(filename, index);

    const std::string value = filename.substr(index, i - index);
    bool isValid = (i == index || i > index + 3);

    int firstNum =  isValid ? 0 : std::stoi(value);
    size_t commaIndex = (i == index || i > index + 3) ? 0 : i + 1;
    return {firstNum, 0, commaIndex};
}

MulNums loadSecondNum(const std::string& filename, MulNums mul_nums) {
    const size_t i = getEndIndexOfNumber(filename, static_cast<int>(mul_nums.commaIndex));

    const std::string value = filename.substr(mul_nums.commaIndex, i - mul_nums.commaIndex);
    int num2 = (i == mul_nums.commaIndex || i > mul_nums.commaIndex + 3) ? 0 : std::stoi(value);

    return {mul_nums.num1, num2, i + 1};
}

int mullItOver(std::string program) {
    std::vector<size_t> positions;
    size_t pos = program.find(MUL_EXPR);

    // find all mul() calls
    while (pos != std::string::npos) {
        positions.push_back(pos + MUL_EXPR.size());
        pos = program.find(MUL_EXPR, pos + MUL_EXPR.length()); // Continue searching
    }

    int result = 0;
    for (int position : positions)
    {
        MulNums mul_nums = loadFirstNum(program, position);
        if(program[mul_nums.commaIndex - 1] != ',') continue;

        mul_nums = loadSecondNum(program, mul_nums);
        if(program[mul_nums.commaIndex - 1] != ')') continue;

        result += mul_nums.num1 * mul_nums.num2;
    }

    return result;
}

// ---------------------------------------------------------------------------------------------

// -------------------------------- Second Part ------------------------------------------------

const std::string DO_EXPR ="do()";
const std::string DONT_EXPR ="don't()";

enum instructions
{
    MUL = 0,
    DO = 1,
    DONT = 2
};

std::vector<size_t> findAllIndexes(std::string program, std::string instruction) {
    std::vector<size_t> positions;
    size_t pos = program.find(instruction);

    // find all mul() calls
    while (pos != std::string::npos) {
        positions.push_back(pos + instruction.size());
        // Continue searching
        pos = program.find(instruction, pos + instruction.length());
    }

    return positions;
}

std::vector<std::pair<instructions, int>> buildQueue(std::string program) {
    const std::vector<size_t> mulPositions = findAllIndexes(program, MUL_EXPR);
    const std::vector<size_t> doPositions = findAllIndexes(program, DO_EXPR);
    const std::vector<size_t> dontPositions = findAllIndexes(program, DONT_EXPR);

    std::vector<std::pair<instructions, int>> result;
    result.reserve(mulPositions.size() + doPositions.size() + dontPositions.size());

    for (size_t position : mulPositions) result.emplace_back(MUL, position);
    for (size_t position : doPositions) result.emplace_back(DO, position);
    for (size_t position : dontPositions) result.emplace_back(DONT, position);

    std::sort(result.begin(), result.end(),
        [](const auto& a, const auto& b) { return a.second < b.second;}
        );

    return result;
}

int mullItOverWithDo(std::string program) {
    std::vector<std::pair<instructions, int>> positions = buildQueue(program);

    int result = 0;
    bool isActive = true;
    for (std::pair<instructions, int> position : positions)
    {
        if (position.first == DONT) isActive = false;
        if (position.first == DO) isActive = true;

        if (position.first == MUL && isActive)
        {
            MulNums mul_nums = loadFirstNum(program, position.second);
            if(program[mul_nums.commaIndex - 1] != ',') continue;

            mul_nums = loadSecondNum(program, mul_nums);
            if(program[mul_nums.commaIndex - 1] != ')') continue;

            result += mul_nums.num1 * mul_nums.num2;
        }
    }

    return result;
}

// ---------------------------------------------------------------------------------------------

int main()
{
    // write working directory
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Current working directory: " << cwd << std::endl;

    // load input data
    const std::string filename = "03/input/input3";
    const std::vector<std::string> listOfLists = loadInput(filename);

    assert(!listOfLists.empty());

    // Part one
    int result = 0;
    for (const auto& report : listOfLists)
    {
        result += mullItOver(report);
    }

    std::cout << "Part 1: " << result << std::endl;

    // Part two
    result = 0;
    for (const auto& report : listOfLists)
    {
        result += mullItOverWithDo(report);
    }

    std::cout << "Part 2: " << result << std::endl;
}
