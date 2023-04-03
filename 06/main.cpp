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

size_t GetUniqueCharacterCountInWindow(const array<size_t, 26>& window)
{
    auto range = window
        | ranges::views::transform([](const size_t count)->unsigned int { return clamp(count, static_cast<size_t>(0), static_cast<size_t>(1)); });

    const size_t uniqueCharactersCount = accumulate(range.begin(), range.end(), 0);

    return uniqueCharactersCount;
}

size_t GetIndexForCharacter(const char inputChar)
{
    return inputChar - 'a';
}

size_t GetPartOneAnswer(const vector<string>& inputLines)
{
    assert(inputLines.size() == 1);

    const string& inputLine = inputLines.front();

    const size_t maxElementsInWindow = 4;
    assert(inputLine.size() > maxElementsInWindow);

    array<size_t, 26> window{};

    for(const size_t index: views::iota(0) | views::take(maxElementsInWindow))
    {
        window[GetIndexForCharacter(inputLine[index])]++;
    }

    const size_t uniqueCharactersCount = GetUniqueCharacterCountInWindow(window);
    if(uniqueCharactersCount == maxElementsInWindow)
        return 4;

    for(const size_t index: views::iota(maxElementsInWindow) | views::take(inputLine.size() - 1))
    {
        window[GetIndexForCharacter(inputLine[index - maxElementsInWindow])]--;
        window[GetIndexForCharacter(inputLine[index])]++;

        const size_t uniqueCharactersCount = GetUniqueCharacterCountInWindow(window);
        if(uniqueCharactersCount == maxElementsInWindow)
            return index + 1;
    }

    assert(false);
    return {};
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneAnswer(inputLines) << endl;

    return 0;
}
