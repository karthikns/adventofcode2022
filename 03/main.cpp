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

uint32_t GetPriority(const array<bool, 53>& first, const array<bool, 53>& second, const array<bool, 53>& third)
{
    for(size_t index = 0; index < first.size(); ++index)
        if(first[index] && second[index] && third[index])
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

uint32_t GetPartTwoScore(const vector<string>& inputLines)
{
    uint32_t score = 0;
    for(size_t index = 0; index + 2 < inputLines.size(); index += 3)
    {
        const string& firstInputLine = inputLines[index];
        const string& secondInputLine = inputLines[index + 1];
        const string& thirdInputLine = inputLines[index + 2];

        const array<bool, 53> firstCharacterPresenceArray = GetCharacterPresenceArray(firstInputLine.begin(), firstInputLine.end());
        const array<bool, 53> secondCharacterPresenceArray = GetCharacterPresenceArray(secondInputLine.begin(), secondInputLine.end());
        const array<bool, 53> thirdCharacterPresenceArray = GetCharacterPresenceArray(thirdInputLine.begin(), thirdInputLine.end());

        score += GetPriority(firstCharacterPresenceArray, secondCharacterPresenceArray, thirdCharacterPresenceArray);
    }

    return score;
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneScore(inputLines) << endl;
    cout << "Part 2: " << GetPartTwoScore(inputLines) << endl;

    return 0;
}