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

    Point<T>& operator=(const Point<T>& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    Point<T> operator+(const Point<T>& other) const
    {
        Point<T> result(x + other.x, y + other.y);
        return result;
    }

    Point<T>& operator+=(const Point<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    T& x = this->first;
    T& y = this->second;
};

Point<int> MoveTail(const Point<int>& newHead, const Point<int>& oldTail)
{
    Point<int> deltaToMoveTail;
    if(newHead.x == oldTail.x || newHead.y == oldTail.y)
    {
        deltaToMoveTail.x = clamp((newHead.x - oldTail.x) / 2, -1, 1);
        deltaToMoveTail.y = clamp((newHead.y - oldTail.y) / 2, -1, 1);
    }
    else if(abs(newHead.x - oldTail.x) == 2 || abs(newHead.y - oldTail.y) == 2)
    {
        deltaToMoveTail.x = clamp((newHead.x - oldTail.x), -1, 1);
        deltaToMoveTail.y = clamp((newHead.y - oldTail.y), -1, 1);
    }

    return oldTail + deltaToMoveTail;
}
size_t SimulateHeadMovementAndGetUniqueTailPositionCount(const vector<pair<char, int>>& instructions, const size_t ropeSize)
{
    assert(ropeSize >= 2);
    vector<Point<int>> rope(ropeSize);
    set<Point<int>> tailVisitLocations;
    tailVisitLocations.emplace(rope.back());

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
            rope.front() = rope.front() + direction;
            for(auto tailIt = rope.begin() + 1; tailIt != rope.end(); ++tailIt)
            {
                Point<int>& tail = *tailIt;
                auto headIt = tailIt;
                --headIt;
                Point<int>& head = *headIt;

                tail = MoveTail(head, tail);
            }

            tailVisitLocations.emplace(rope.back());
        }
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
    return SimulateHeadMovementAndGetUniqueTailPositionCount(instructions, 2);
}

size_t GetPartTwoAnswer(const vector<string>& inputLines)
{
    auto range = inputLines
        | views::transform([](const string& inputLine) {
            pair<char, int> p;
            sscanf(inputLine.c_str(), "%c %d", &p.first, &p.second);
            return p;
        })
        ;

    vector<pair<char, int>> instructions(range.begin(), range.end());
    return SimulateHeadMovementAndGetUniqueTailPositionCount(instructions, 10);
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneAnswer(inputLines) << endl;
    cout << "Part 2: " << GetPartTwoAnswer(inputLines) << endl;

    return 0;
}
