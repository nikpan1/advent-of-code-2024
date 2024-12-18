//
// Created by nikod on 01/12/2024.
//

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <bits/ranges_util.h>

using ListOfLists = std::vector<std::vector<int>>;

ListOfLists loadInput(const std::string& filename)
{
    std::vector<std::vector<int>> result;
    std::ifstream file(filename);

    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<int> row;
        std::stringstream ss(line);
        int num;

        while (ss >> num)
        {
            row.push_back(num);
        }
        result.push_back(row);
    }

    return result;
}

// -------------------------------- First Part ------------------------------------------------

constexpr int MAX_DISTANCE = 3;
constexpr int MIN_DISTANCE = 1;

bool isInRange(int value)
{
    return MIN_DISTANCE <= abs(value) && abs(value) <= MAX_DISTANCE;
}

bool isNumberPositive(int number)
{
    return number >= 0;
}

bool isReportSave(const std::vector<int>& report)
{
    bool coefficient = isNumberPositive(report[0] - report[1]);
    for (int i = 0; i < report.size() - 1; i++)
    {
        int distance = report[i] - report[i + 1];
        if (isNumberPositive(distance) != coefficient) return false;
        if (!isInRange(distance)) return false;
    }

    return true;
}

// ---------------------------------------------------------------------------------------------

// -------------------------------- Second Part ------------------------------------------------

bool isReportSaveForSure(const std::vector<int>& report)
{
    std::vector<int> distances;
    distances.reserve(report.size() - 1);

    for (int i = 0; i < report.size() - 1; i++)
    {
        distances.push_back(report[i] - report[i + 1]);
    }

    int un = 0;
    int positives = 0, negatives = 0;
    for (const auto distance : distances)
    {
        if (!isInRange(distance)) un++;
        if (distance >= 0) positives++;
        if (distance < 0) negatives++;
    }

    return un < 2 && std::min(positives, negatives) < 2;
}

// ---------------------------------------------------------------------------------------------

int main()
{
    // write working directory
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Current working directory: " << cwd << std::endl;

    // load input data
    const std::string filename = "02/input/input1";
    const ListOfLists listOfLists = loadInput(filename);
    assert(!listOfLists.empty());

    // Part one
    int result = 0;
    for (const auto& report : listOfLists)
    {
        result += isReportSave(report);
    }

    std::cout << "Part 1: " << result << std::endl;

    // Part two
    result = 0;
    for (const auto& report : listOfLists)
    {
        result += isReportSaveForSure(report);
    }

    std::cout << "Part 2: " << result << std::endl;
}
