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

struct Instruction
{
    unsigned int count;
    unsigned int from;
    unsigned int to;
};

vector<vector<char>> GetEmptyStacks(const string& stackLabels)
{
    size_t labelCount = 0;
    stringstream labelStream(stackLabels);
    string label;
    while(getline(labelStream, label, ' '))
    {
        if(label.empty())
            continue;

        ++labelCount;
    }

    return vector<vector<char>>(labelCount + 1);
}

vector<vector<char>> GetInitialStacksState(ranges::input_range auto&& stackInputRange)
{
    auto reverseInputRange = stackInputRange | ranges::views::reverse;

    string stackLabels = *((reverseInputRange | views::take(1)).begin());
    vector<vector<char>> stacks = GetEmptyStacks(stackLabels);

    auto stackValuesInputRange = reverseInputRange | views::drop(1);

    for(const string& stackValues: stackValuesInputRange)
    {
        size_t stackIndex = 0;
        for(size_t index = 1; index < stackValues.size(); index += 4)
        {
            ++stackIndex;
            if(stackValues[index] == ' ')
                continue;

            stacks[stackIndex].push_back(stackValues[index]);
        }
    }

    return stacks;
}

Instruction GetInstructionFromString(const string& instructionString)
{
    Instruction instruction;
    sscanf(instructionString.c_str(), "move %d from %d to %d", &instruction.count, &instruction.from, &instruction.to);
    return instruction;
}

vector<Instruction> GetInstructions(ranges::input_range auto&& instructionsInputRange)
{
    auto instructionsRange = instructionsInputRange
    | ranges::views::transform(GetInstructionFromString);

    return vector<Instruction>(instructionsRange.begin(), instructionsRange.end());
}

void ProcessInstructionsPartOne(vector<vector<char>>& stacks, const vector<Instruction>& instructions)
{
    for(const Instruction& instruction: instructions)
    {
        for(size_t index = 0; index < instruction.count; ++index)
        {
            stacks[instruction.to].push_back(stacks[instruction.from].back());
            stacks[instruction.from].pop_back();
        }
    }
}

void ProcessInstructionsPartTwo(vector<vector<char>>& stacks, const vector<Instruction>& instructions)
{
    for(const Instruction& instruction: instructions)
    {
        const unsigned int itemsToMove = instruction.count;
        vector<char>& fromStack = stacks[instruction.from];
        vector<char>& toStack = stacks[instruction.to];

        auto itemsToMoveBegin = fromStack.end() - instruction.count;
        toStack.insert(toStack.end(), itemsToMoveBegin, fromStack.end());
        fromStack.erase(itemsToMoveBegin, fromStack.end());
    }
}

using InstructionProcessorFunction = function<void(vector<vector<char>>&, const vector<Instruction>&)>;

string GetAnswer(const vector<string>& inputLines, InstructionProcessorFunction instructionProcessorFunction)
{
    auto stacksInputLines =
        inputLines
        | views::take_while([](string str){ return !str.empty(); })
        ;

    vector<vector<char>> stacks = GetInitialStacksState(stacksInputLines);

    auto instructionsInputLines =
        inputLines
        | views::reverse
        | views::take_while([](string str){ return !str.empty(); })
        | views::reverse
        ;

    vector<Instruction> instructions = GetInstructions(instructionsInputLines);

    instructionProcessorFunction(stacks, instructions);

    string topOfStacks;
    for(size_t index = 1; index < stacks.size(); ++index)
        topOfStacks += stacks[index].back();

    return topOfStacks;
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    InstructionProcessorFunction instructionProcessorFunction;

    instructionProcessorFunction = ProcessInstructionsPartOne;
    cout << "Part 1: " << GetAnswer(inputLines, instructionProcessorFunction) << endl;

    instructionProcessorFunction = ProcessInstructionsPartTwo;
    cout << "Part 2: " << GetAnswer(inputLines, instructionProcessorFunction) << endl;

    return 0;
}
