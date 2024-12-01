//
// Created by nikod on 01/12/2024.
//

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <utility>

using LocationIDLists = std::pair<std::vector<int>, std::vector<int>>;
using score = int;

LocationIDLists readFileToColumns(const std::string& filename) {
    LocationIDLists locationIDLists;
    std::ifstream inputFile(filename);

    if (!inputFile) {
        throw std::runtime_error("Error: Could not open the file!");
    }

    int firstValue, secondValue;
    while (inputFile >> firstValue >> secondValue) {
        locationIDLists.first.push_back(firstValue);
        locationIDLists.second.push_back(secondValue);
    }

    inputFile.close();
    return locationIDLists;
}

// -------------------------------- First Part ------------------------------------------------

int calculateDistance(const LocationIDLists& lists) {
    int summarizedDistances = 0;

    for (int i = 0; i < lists.first.size(); i++) {
        summarizedDistances += abs(lists.first[i] - lists.second[i]);
    }

    return summarizedDistances;
}

// ---------------------------------------------------------------------------------------------


// -------------------------------- Second Part ------------------------------------------------


bool isKeyInMapInitialized(const std::vector<int>& list, const std::map<int, score>& similarity, int i)
{
    return similarity.contains(list[i]);
}

int calculateSimilarityScore(const LocationIDLists& lists) {
    std::pair<std::map<int, score>, std::map<int, score>> similarities;

    for (int i = 0; i < lists.first.size(); i++)
    {
        int firstRecord = lists.first[i];
        similarities.first[firstRecord] = isKeyInMapInitialized(lists.first, similarities.first, i) ?
            similarities.first[firstRecord] + 1 : 1;

        int secondRecord = lists.second[i];
        similarities.second[secondRecord] = isKeyInMapInitialized(lists.second, similarities.second, i) ?
            similarities.second[secondRecord] + 1 : 1;
    }

    int result = 0;
    for (const auto& pair : similarities.first) {
        result += pair.first * pair.second * similarities.second[pair.first];
    }

    return result;
}

// ---------------------------------------------------------------------------------------------


int main() {
    const std::string filename = "../01/input/input2";
    LocationIDLists locationIDLists = readFileToColumns(filename);

    // check if correct input size
    assert(!locationIDLists.first.empty() &&
        !locationIDLists.second.empty() &&
        locationIDLists.second.size() == locationIDLists.second.size());

    // Part 1
    std::ranges::sort(locationIDLists.first);
    std::ranges::sort(locationIDLists.second);

    int calculatedDistances = calculateDistance(locationIDLists);

    std::cout << calculatedDistances << std::endl;

    // Part 2
    int similarityScore = calculateSimilarityScore(locationIDLists);

    std::cout << similarityScore << std::endl;

    return 0;
}
