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

bool DoSectionsOverlap(pair<Section, Section> elvesPair)
{
    if(elvesPair.first.sectionBegin == elvesPair.second.sectionBegin)
        return true;

    if(elvesPair.second.sectionBegin < elvesPair.first.sectionBegin)
        swap(elvesPair.first, elvesPair.second);

    if(elvesPair.second.sectionEnd <= elvesPair.first.sectionEnd)
        return true;

    return false;
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
        | ranges::views::transform(DoSectionsOverlap)
        | ranges::views::transform([](const bool isOverlapping){ return static_cast<uint32_t>(isOverlapping); });

    return accumulate(overlappingPairs.begin(), overlappingPairs.end(), 0);
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneAnswer(inputLines) << endl;

    return 0;
}