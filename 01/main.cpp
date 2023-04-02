#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <ranges>
#include <vector>

using namespace std;

vector<string> GetFileContents()
{
    const string inputFileName = "input.txt";
    ifstream fileStream(inputFileName, ios::in);

    vector<string> input;

    string line;
    while(getline(fileStream, line, '\n'))
        input.emplace_back(line);

    return input;
}

vector<uint64_t> GetCaloriesCountPerElf(const vector<string>& fileContents)
{
    vector<uint64_t> caloriesCountPerElf;

    uint64_t calorieCount = 0;
    auto beginIt = fileContents.begin();
    auto endIt = beginIt;

    while(beginIt != fileContents.end())
    {
        auto endIt = find(beginIt, fileContents.end(), string());

        auto numbers =
            ranges::subrange(beginIt, endIt) |
            ranges::views::transform([](string str) { return strtol(str.c_str(), nullptr, 10); } );

        caloriesCountPerElf.emplace_back(accumulate(begin(numbers), end(numbers), 0));

        if(endIt == fileContents.end())
            break;

        beginIt = endIt + 1;
    }

    return caloriesCountPerElf;
}

uint64_t GetMaxCaloriesCountElf(const vector<uint64_t>& caloriesCountPerElf)
{
    return *max_element(begin(caloriesCountPerElf), end(caloriesCountPerElf));
}

uint64_t GetMax3CaloriesCountElves(const vector<uint64_t>& caloriesCountPerElf)
{
    if(caloriesCountPerElf.size() < 3)
        return accumulate(begin(caloriesCountPerElf), end(caloriesCountPerElf), 0);

    auto range = caloriesCountPerElf | ranges::views::take(3);
    vector<uint64_t> heap(range.begin(), range.end());
    make_heap(heap.begin(), heap.end());

    for(auto it = caloriesCountPerElf.begin() + 3; it != caloriesCountPerElf.end(); ++it)
    {
        auto comp = [](uint64_t num1, uint64_t num2) { return num2 < num1; };

        heap.push_back(*it);
        push_heap(begin(heap), end(heap), comp);

        pop_heap(begin(heap), end(heap), comp);
        heap.pop_back();
    }

    return accumulate(heap.begin(), heap.end(), 0);
}

int main()
{
    vector<string> fileContents = GetFileContents();

    cout << "Computing GetCaloriesCountPerElf()" << endl;
    vector<uint64_t> caloriesCountPerElf = GetCaloriesCountPerElf(fileContents);

    cout << "Part 1: " << GetMaxCaloriesCountElf(caloriesCountPerElf) << endl;
    cout << "Part 2: " << GetMax3CaloriesCountElves(caloriesCountPerElf) << endl;

    return 0;
}