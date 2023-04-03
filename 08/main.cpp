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

vector<int> GetTreeHeights(const string& inputLine)
{
    auto range = inputLine | views::transform([](const char c)->int { return c - '0'; });
    return vector<int>(range.begin(), range.end());
}

size_t GetNumberOfTreesVisible(const vector<vector<int>>& grid)
{
    vector<vector<bool>> isVisibleGrid(grid.size(), vector<bool>(grid.front().size(), false));

    vector<bool>& firstRow = isVisibleGrid.front();
    vector<bool>& lastRow = isVisibleGrid.back();

    fill(firstRow.begin(), firstRow.end(), true);
    fill(lastRow.begin(), lastRow.end(), true);

    for(vector<bool>& row: isVisibleGrid)
    {
        row.front() = true;
        row.back() = true;
    }

    const size_t rowCount = grid.size();
    const size_t columnCount = grid.front().size();

    for(const size_t rowIndex : views::iota(1) | views::take(rowCount - 1))
    {
        // Run left -> right
        size_t runningTallestTree = grid[rowIndex].front();
        for(const size_t columnIndex: views::iota(1) | views::take(columnCount - 1))
        {
            if(grid[rowIndex][columnIndex] > runningTallestTree)
            {
                isVisibleGrid[rowIndex][columnIndex] = true;
                runningTallestTree = grid[rowIndex][columnIndex];
            }
        }

        // Run right -> left
        runningTallestTree = grid[rowIndex].back();
        for(const size_t columnIndex: views::iota(1) | views::take(columnCount - 1) | views::reverse)
        {
            if(grid[rowIndex][columnIndex] > runningTallestTree)
            {
                isVisibleGrid[rowIndex][columnIndex] = true;
                runningTallestTree = grid[rowIndex][columnIndex];
            }
        }
    }

    for(const size_t columnIndex: views::iota(1) | views::take(columnCount - 1))
    {
        //  Run top -> bottom
        size_t runningTallestTree = grid.front()[columnIndex];
        for(const size_t rowIndex: views::iota(1) | views::take(rowCount - 1))
        {
            if(grid[rowIndex][columnIndex] > runningTallestTree)
            {
                isVisibleGrid[rowIndex][columnIndex] = true;
                runningTallestTree = grid[rowIndex][columnIndex];
            }
        }

        //  Run bottom -> top
        runningTallestTree = grid.back()[columnIndex];
        for(const size_t rowIndex: views::iota(1) | views::take(rowCount - 1) | views::reverse)
        {
            if(grid[rowIndex][columnIndex] > runningTallestTree)
            {
                isVisibleGrid[rowIndex][columnIndex] = true;
                runningTallestTree = grid[rowIndex][columnIndex];
            }
        }
    }

    auto range = isVisibleGrid
        | views::transform([](const vector<bool>& isVisibleRow){ return count(isVisibleRow.begin(), isVisibleRow.end(), true); })
        ;
    return accumulate(range.begin(), range.end(), 0);
}

size_t GetPartOneAnswer(const vector<string>& inputLines)
{
    auto range = inputLines | views::transform([](const string& inputLine) { return GetTreeHeights(inputLine); });
    vector<vector<int>> grid(range.begin(), range.end());
    return GetNumberOfTreesVisible(grid);
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneAnswer(inputLines) << endl;

    return 0;
}
