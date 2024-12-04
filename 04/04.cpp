//
// Created by nikod on 01/12/2024.
//

//
// Created by nikod on 01/12/2024.
//

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <filesystem>

std::vector<std::vector<char>> loadInput(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<std::vector<char>> result;
    std::string line;
    while (std::getline(file, line))
    {
        result.emplace_back(line.begin(), line.end());
    }

    return result;
}

// -------------------------------- First Part ------------------------------------------------

const std::vector<std::pair<int, int>> directions = {
    {-1, -1}, {-1, 0}, {-1, 1},   // Top
    { 0, -1},                 { 0, 1},   // Middle
    { 1, -1}, { 1, 0}, { 1, 1}    // Bottom
};

std::vector<std::pair<int, int>> findAllX(const std::vector<std::vector<char>>& input, const char c='X')
{
    std::vector<std::pair<int, int>> positions;

    for (int x = 0; x < input.size(); x++)
    {
        for (int y = 0; y < input[x].size(); y++)
        {
            if (input[x][y] == c)
            {
                positions.emplace_back(x, y);
            }
        }
    }

    return positions;
}

bool isInBound(const std::vector<std::vector<char>>& input, int nx, int ny)
{
    return nx >= 0 && ny >= 0 && nx < input.size() && ny < input[0].size();
}

std::vector<std::pair<int, int>> findAllMAround(const std::vector<std::vector<char>>& input, int x, int y)
{
    std::vector<std::pair<int, int>> foundM;
    for (auto [dx, dy] : directions)
    {
        const int nx = x + dx;
        const int ny = y + dy;

        if (isInBound(input, nx, ny) && input[nx][ny] == 'M')
        {
            foundM.emplace_back(nx, ny);
        }
    }

    return foundM;
}

bool isNotInBound(const std::vector<std::vector<char>>& input, const std::pair<int, int>& vec)
{
    int row = vec.second;
    int col = vec.first;
    return row < 0 || row >= input.size() || col < 0 || col >= input[row].size();
}

const std::vector<char> restOfXMAS= {'A', 'S'};

int findAllXMASS(const std::vector<std::vector<char>>& input)
{
    int result = 0;
    auto allX = findAllX(input);
    for (const auto& [x, y] : allX)
    {
        auto allFoundM = findAllMAround(input, x, y);
        for (const auto& [mx, my] : allFoundM)
        {
            bool isFound = true;
            const std::pair vec = {mx - x, my - y};
            std::pair pivot = {mx, my};

            for (const auto c : restOfXMAS)
            {
                pivot.first += vec.first;
                pivot.second += vec.second;

                if (isNotInBound(input, pivot) || input[pivot.first][pivot.second] != c)
                {
                    isFound = false;
                    break;
                }
            }

            if (isFound) result++;
        }
    }
    return result;
}

// ---------------------------------------------------------------------------------------------

// -------------------------------- Second Part ------------------------------------------------

struct pos
{
    int x;
    int y;
};
struct opp
{
    pos pos1;
    pos pos2;
};

const std::vector<opp> corners = {
    {{-1, -1}, { 1, -1}},
    {{-1, 1 }, { 1, 1 }}
};

std::vector<std::pair<int, int>> findAllAAround(const std::vector<std::vector<char>>& input, int x, int y)
{
    std::vector<std::pair<int, int>> foundM;
    for (auto [dx, dy] : directions)
    {
        const int nx = x + dx;
        const int ny = y + dy;

        if (isInBound(input, nx, ny) && input[nx][ny] == 'M')
        {
            foundM.emplace_back(nx, ny);
        }
    }

    return foundM;
}

int findAllCrossedMAS(const std::vector<std::vector<char>>& input)
{
    int result = 0;
    auto allX = findAllX(input, 'A'); // Assuming you find positions of 'A' correctly
    for (const auto& [x, y] : allX)
    {
        bool isFound = true;

        // Check the surrounding positions using the corners
        for (const auto& [pos1, pos2] : corners)
        {
            int nx1 = x + pos1.x, ny1 = y + pos1.y;
            int nx2 = x + pos2.x, ny2 = y + pos2.y;

            // Check if the positions are within bounds
            if (!isInBound(input, nx1, ny1) || !isInBound(input, nx2, ny2)) {
                isFound = false;
                break;
            }
        }

        if (!isFound) continue;

        // Now check the "MAS" pattern at the corners
        // You should use correct positions for each corner based on your description
        if (input[x + corners[0].pos1.x][y + corners[0].pos1.y] == input[x + corners[0].pos2.x][y + corners[0].pos2.y] && // Opp 1 is equal
            input[x + corners[1].pos1.x][y + corners[1].pos1.y] == input[x + corners[1].pos2.x][y + corners[1].pos2.y] && // Opp 2 is equal
            input[x + corners[0].pos1.x][y + corners[0].pos1.y] != input[x + corners[1].pos1.x][y + corners[1].pos1.y] && // Both opposites are different
            (input[x + corners[0].pos1.x][y + corners[0].pos1.y] == 'M' || input[x + corners[0].pos1.x][y + corners[0].pos1.y] == 'S') && // Check for 'M' or 'S'
            (input[x + corners[1].pos1.x][y + corners[1].pos1.y] == 'M' || input[x + corners[1].pos1.x][y + corners[1].pos1.y] == 'S'))
        {
            result++;
        }
    }
    return result;
}


// ---------------------------------------------------------------------------------------------

int main()
{
    std::cout << "Day 4!!!" << std::endl;

    // print current working directory
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Current working directory: " << cwd << std::endl;

    // load input data
    const std::string filename = "04/input/input3";
    const std::vector<std::vector<char>> crossword = loadInput(filename);
    assert(!crossword.empty());

    // Part one
    int result = findAllXMASS(crossword);

    std::cout << "Part 1: " << result << std::endl;

    // Part two
    result = findAllCrossedMAS(crossword);

    std::cout << "Part 2: " << result << std::endl;
}
