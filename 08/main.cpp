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

size_t GetMaxScenicScore(const vector<vector<int>>& grid)
{
    const size_t rowCount = grid.size();
    const size_t columnCount = grid.front().size();

    size_t maxScenicScore = 0;
    for(const size_t rowIndex: views::iota(1) | views::take(rowCount - 2))
    {
        for(const size_t columnIndex: views::iota(1) | views::take(columnCount - 2))
        {
            const size_t currentTreeValue = grid[rowIndex][columnIndex];

            // travel left
            size_t leftScenicScore = 0;
            for(long leftRunner = columnIndex - 1; leftRunner >= 0; --leftRunner)
            {
                ++leftScenicScore;
                if(grid[rowIndex][leftRunner] >= currentTreeValue)
                    break;
            }

            // travel right
            size_t rightScenicScore = 0;
            for(long rightRunner = columnIndex + 1; rightRunner < columnCount; ++rightRunner)
            {
                ++rightScenicScore;
                if(grid[rowIndex][rightRunner] >= currentTreeValue)
                    break;
            }

            // travel top
            size_t topScenicScore = 0;
            for(long topRunner = rowIndex - 1; topRunner >= 0; --topRunner)
            {
                ++topScenicScore;
                if(grid[topRunner][columnIndex] >= currentTreeValue)
                    break;
            }

            // travel bottom
            size_t bottomScenicScore = 0;
            for(long bottomRunner = rowIndex + 1; bottomRunner < rowCount; ++bottomRunner)
            {
                ++bottomScenicScore;
                if(grid[bottomRunner][columnIndex] >= currentTreeValue)
                    break;
            }

            const size_t scenicScore = leftScenicScore * rightScenicScore * topScenicScore * bottomScenicScore;
            maxScenicScore = max(maxScenicScore, scenicScore);
        }
    }

    return maxScenicScore;
}

size_t GetPartTwoAnswer(const vector<string>& inputLines)
{
    auto range = inputLines | views::transform([](const string& inputLine) { return GetTreeHeights(inputLine); });
    vector<vector<int>> grid(range.begin(), range.end());
    return GetMaxScenicScore(grid);
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneAnswer(inputLines) << endl;
    cout << "Part 2: " << GetPartTwoAnswer(inputLines) << endl;

    return 0;
}
