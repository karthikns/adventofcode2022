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

struct Section 
{
    size_t sectionBegin;
    size_t sectionEnd;
};

bool DoesOneSectionContainTheOther(pair<Section, Section> elvesPair)
{
    if(elvesPair.first.sectionBegin == elvesPair.second.sectionBegin)
        return true;

    if(elvesPair.second.sectionBegin < elvesPair.first.sectionBegin)
        swap(elvesPair.first, elvesPair.second);

    if(elvesPair.second.sectionEnd <= elvesPair.first.sectionEnd)
        return true;

    return false;
}

bool DoSectionsIntersect(pair<Section, Section> elvesPair)
{
    if(elvesPair.first.sectionBegin == elvesPair.second.sectionBegin)
        return true;

    if(elvesPair.second.sectionBegin < elvesPair.first.sectionBegin)
        swap(elvesPair.first, elvesPair.second);

    if(elvesPair.first.sectionEnd < elvesPair.second.sectionBegin)
        return false;

    return true;
}

pair<Section, Section> GetElvesPair(const string& inputLine)
{
    pair<Section, Section> elvesPair;
    sscanf(inputLine.c_str(), "%ld-%ld,%ld-%ld",
        &elvesPair.first.sectionBegin, &elvesPair.first.sectionEnd,
        &elvesPair.second.sectionBegin, &elvesPair.second.sectionEnd);
    return elvesPair;
}

uint32_t GetPartOneAnswer(const vector<string>& inputLines)
{
    auto overlappingPairs = inputLines
        | ranges::views::transform(GetElvesPair)
        | ranges::views::transform(DoesOneSectionContainTheOther)
        | ranges::views::transform([](const bool boolValue){ return static_cast<uint32_t>(boolValue); })
        ;

    return accumulate(overlappingPairs.begin(), overlappingPairs.end(), 0);
}

uint32_t GetPartTwoAnswer(const vector<string>& inputLines)
{
    auto overlappingPairs = inputLines
        | ranges::views::transform(GetElvesPair)
        | ranges::views::transform(DoSectionsIntersect)
        | ranges::views::transform([](const bool boolValue){ return static_cast<uint32_t>(boolValue); })
        ;

    return accumulate(overlappingPairs.begin(), overlappingPairs.end(), 0);
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneAnswer(inputLines) << endl;
    cout << "Part 2: " << GetPartTwoAnswer(inputLines) << endl;

    return 0;
}
