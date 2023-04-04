#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
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

template<typename T>
class Point: public pair<T, T>
{
public:
    Point(): pair<T, T>()
    {
    }

    Point(const T& x, const T& y): pair<T, T>(x, y)
    {
    }

    Point(T&& x, T&& y): pair<T, T>(x, y)
    {
    }

    T& x = this->first;
    T& y = this->second;
};

size_t SimulateHeadMovementAndGetUniqueTailPositionCount(const vector<pair<char, int>>& instructions)
{
    Point<int> head = { 0, 0 };
    Point<int> tail = { 0, 0 };

    set<Point<int>> tailVisitLocations;
    tailVisitLocations.emplace(tail);

    for(const pair<char, int>& instruction: instructions)
    {
        Point<int> direction{0,0};

        switch(instruction.first)
        {
            case 'R':
                ++direction.x;
                break;

            case 'L':
                --direction.x;
                break;

            case 'U':
                ++direction.y;
                break;

            case 'D':
                --direction.y;
                break;
        }

        for(size_t index: views::iota(1) | views::take(instruction.second))
        {
            head.x += direction.x;
            head.y += direction.y;

            if(head == tail)
                continue;

            Point<int> deltaToMoveTail;
            if(head.x == tail.x || head.y == tail.y)
            {
                deltaToMoveTail.x = clamp((head.x - tail.x) / 2, -1, 1);
                deltaToMoveTail.y = clamp((head.y - tail.y) / 2, -1, 1);
            }
            else if(abs(head.x - tail.x) == 2 || abs(head.y - tail.y) == 2)
            {
                deltaToMoveTail.x = clamp((head.x - tail.x), -1, 1);
                deltaToMoveTail.y = clamp((head.y - tail.y), -1, 1);
            }

            tail.x += deltaToMoveTail.x;
            tail.y += deltaToMoveTail.y;

            tailVisitLocations.emplace(tail);
        }

        cout << endl;
    }

    return tailVisitLocations.size();
}

size_t GetPartOneAnswer(const vector<string>& inputLines)
{
    auto range = inputLines
        | views::transform([](const string& inputLine) {
            pair<char, int> p;
            sscanf(inputLine.c_str(), "%c %d", &p.first, &p.second);
            return p;
        })
        ;

    vector<pair<char, int>> instructions(range.begin(), range.end());
    return SimulateHeadMovementAndGetUniqueTailPositionCount(instructions);
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneAnswer(inputLines) << endl;

    return 0;
}
