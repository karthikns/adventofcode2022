#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

vector<string> GetFileContents(const string& fileName)
{
    ifstream fileStream(fileName, ios::in);

    vector<string> input;

    string line;
    while(getline(fileStream, line, '\n'))
        input.emplace_back(line);

    return input;
}

uint32_t GetScore(const char ch)
{
    uint32_t score = 0;
    assert(isalpha(ch));
    if(isupper(ch))
        score += 26;

    score += (tolower(ch) - 'a') + 1;

    return score;
}

array<bool, 53> GetCharacterPresenceArray(string::const_iterator begin, string::const_iterator end)
{
    array<bool, 53> characterPresenceArray = {};
    for(; begin != end; ++begin)
        characterPresenceArray[GetScore(*begin)] = true;
    return characterPresenceArray;
}

uint32_t GetPriority(const array<bool, 53>& first, const array<bool, 53>& second)
{
    for(size_t index = 0; index < first.size(); ++index)
        if(first[index] && second[index])
            return index;

    assert(false);
    return {};
}

uint32_t GetPartOneScore(const vector<string>& inputLines)
{
    uint32_t score = 0;
    for(const string& inputLine: inputLines)
    {
        const size_t inputLineSize = inputLine.size();
        assert(inputLineSize && inputLineSize % 2 == 0);

        auto midIt = inputLine.begin() + (inputLineSize / 2);
        const array<bool, 53> firstHalfCharacterPresenceArray = GetCharacterPresenceArray(inputLine.begin(), midIt);
        const array<bool, 53> secondHalfCharacterPresenceArray = GetCharacterPresenceArray(midIt, inputLine.end());

        score += GetPriority(firstHalfCharacterPresenceArray, secondHalfCharacterPresenceArray);
    }

    return score;
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneScore(inputLines) << endl;

    return 0;
}