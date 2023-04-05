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

enum class InstructionEnum : unsigned int
{
    addx,
    noop
};

struct Instruction
{
    InstructionEnum instructionCode;
    int intValue;
};

Instruction GetInstructionFromInputLine(const string& inputLine)
{
    vector<string> instructionParts;
    stringstream instructionInputStringnStream(inputLine);
    string candidate;
    while(getline(instructionInputStringnStream, candidate, ' '))
    {
        if(candidate.empty())
            continue;

        instructionParts.emplace_back(candidate);
    }

    if(instructionParts[0] == "noop")
    {
        return { InstructionEnum::noop, 0 };
    }

    if(instructionParts[0] == "addx")
    {
        Instruction addxInstruction = { InstructionEnum::addx, 0 };
        sscanf(instructionParts[1].c_str(), "%d", &addxInstruction.intValue);
        return addxInstruction;
    }

    assert(false);
    return {};
}

map<InstructionEnum, size_t> instructionToCycleCountMap =
    {
        { InstructionEnum::noop, 1},
        { InstructionEnum::addx, 2}
    };

size_t ProcessInstructionsAndGetPartOneAnswer(const vector<Instruction>& instructions)
{
    int X = 1;

    vector<Instruction>::const_iterator it = instructions.begin();
    Instruction currentExecutingInstruction{};
    vector<int> xHistory = { 0, X };
    int cyclesRemaining = 0;
    for(size_t cycleIndex = 1; cycleIndex <= 5000; ++cycleIndex)
    {
        if(cyclesRemaining > 0)
        {
            xHistory.push_back(X);
            --cyclesRemaining;
            continue;
        }

        if(1 == cycleIndex)
            it = instructions.begin();
        else
        {
            if(currentExecutingInstruction.instructionCode == InstructionEnum::addx)
                X += currentExecutingInstruction.intValue;
            xHistory.push_back(X);

            ++it;
        }

        if(it == instructions.end())
            break;

        currentExecutingInstruction = *it;
        cyclesRemaining = instructionToCycleCountMap[currentExecutingInstruction.instructionCode];

        --cyclesRemaining;
    }

    size_t signalStrength = 0;
    for(size_t index = 20; index < xHistory.size(); index += 40)
        signalStrength += (index * xHistory[index]);

    return signalStrength;
}

void ProcessInstructionsAndGetPartTwoAnswer(const vector<Instruction>& instructions)
{
    int X = 1;
    optional<Instruction> instructionBeingProcessed;
    int nextInstructionIndex = 0;
    int cyclesRemaining = 0;
    vector<int> xHistory = { 0 };

    string spritePosition(40, '.');
    auto range = views::iota(X - 1) | views::take(3) | views::filter([](const int num) { return num >= 0 && num < 40; });
    for(const int index: range)
        spritePosition[index] = '#';

    string image(240, '?');

    for(size_t cycleIndex = 1; cycleIndex <= 5000; ++cycleIndex)
    {
        // Pre-processing:
        // Get instruction being processed
        // Is instruction is being processed?
        // No:
        //      Get next instruction
        //      Set instruction being processed to the next instruction
        if(!instructionBeingProcessed.has_value())
        {
            if(nextInstructionIndex >= instructions.size())
                break;

            Instruction nextInstruction = instructions[nextInstructionIndex];
            nextInstructionIndex++;

            instructionBeingProcessed = nextInstruction;
            cyclesRemaining = instructionToCycleCountMap[nextInstruction.instructionCode];
        }

        // Processing:
        image[cycleIndex - 1] = spritePosition[(cycleIndex - 1) % 40];
        xHistory.push_back(X);

        // Post-processing:
        // Get instruction being processed
        // Reduce instruction cycles by 1
        // Is instruction cycles 0?
        //      Process instruction
        //      Instruction being processed = nullptr
        --cyclesRemaining;
        if(cyclesRemaining == 0)
        {
            // Process instruction
            if(instructionBeingProcessed->instructionCode == InstructionEnum::addx)
            {
                X += instructionBeingProcessed->intValue;

                spritePosition = string(40, '.');
                auto range = views::iota(X - 1) | views::take(3) | views::filter([](const int num) { return num >= 0 && num < 40; });
                for(const int index: range)
                    spritePosition[index] = '#';
            }

            instructionBeingProcessed.reset();
        }
    }

    auto imageRange = views::iota(0) 
        | views::take(6) 
        | views::transform([image](const int index) { return image.substr(40 * index, 40); })
        ;

    for(const string& imageLine : imageRange)
        cout << imageLine << endl;
    return;
}

size_t GetPartOneAnswer(const vector<string>& inputLines)
{
    auto range = inputLines | views::transform(GetInstructionFromInputLine);
    vector<Instruction> instructions(range.begin(), range.end());

    return ProcessInstructionsAndGetPartOneAnswer(instructions);
}

void GetPartTwoAnswer(const vector<string>& inputLines)
{
    auto range = inputLines | views::transform(GetInstructionFromInputLine);
    vector<Instruction> instructions(range.begin(), range.end());

    ProcessInstructionsAndGetPartTwoAnswer(instructions);
    return;
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneAnswer(inputLines) << endl;
    cout << "Part 2: " << endl;
    GetPartTwoAnswer(inputLines);
    cout << endl;

    return 0;
}
