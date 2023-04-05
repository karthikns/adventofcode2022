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

enum class OperandType : uint8_t
{
    Variable,
    Constant
};

struct Operation
{
    function<size_t(size_t, size_t)> operationFn;
    OperandType operandOneType;
    OperandType operandTwoType;
    size_t operationConstant = 0;

    size_t Process(const size_t worry)
    {
        const size_t operandOne = operandOneType == OperandType::Variable ? worry : operationConstant;
        const size_t operandTwo = operandTwoType == OperandType::Variable ? worry : operationConstant;
        return operationFn(operandOne, operandTwo);
    }
};

struct Test
{
    size_t testDivisibleBy;
    size_t monkeyIndexToThrowWhenTestIsTrue;
    size_t monkeyIndexToThrowWhenTestIsFalse;
};

struct Monkey
{
    vector<size_t> startingItemsWorry;
    Operation operation;
    Test test;

    size_t itemsInspected = 0;
};

//  Starting items: 54, 65, 75, 74
vector<size_t> GetStartingItemsWorry(const string& worryString)
{
    vector<size_t> startingItemsWorry;

    stringstream numbersStream(worryString.substr(17));
    string candidate;
    while(getline(numbersStream, candidate, ','))
    {
        size_t startingItemWorry = stoi(candidate);
        startingItemsWorry.emplace_back(startingItemWorry);
    }

    return startingItemsWorry;
}

bool IsDigits(const string str)
{
    for(const char c: str)
        if(!isdigit(c))
            return false;

    return true;
}

//  Operation: new = old * 17
Operation GetOperation(const string& operationString)
{
    Operation operation;
    char operandOne[16] = {};
    char operandTwo[16] = {};
    char operatorType;

    sscanf(operationString.c_str(), "  Operation: new = %s %c %s", operandOne, &operatorType, operandTwo);
    if(operatorType == '+')
        operation.operationFn = [](int a, int b) { return a + b; };
    else if(operatorType == '*')
        operation.operationFn = [](int a, int b) { return a * b; };

    operation.operandOneType = OperandType::Variable;
    operation.operandTwoType = OperandType::Variable;
    if(IsDigits(operandOne))
    {
        operation.operandOneType = OperandType::Constant;
        operation.operationConstant = stoi(operandOne);
    }
    else if(IsDigits(operandTwo))
    {
        operation.operandTwoType = OperandType::Constant;
        operation.operationConstant = stoi(operandTwo);
    }

    return operation;
}
//  Test: divisible by 19
Test GetTest(const vector<string>& testStrings)
{
    Test test;

    char divisibleByString[16] = {};
    sscanf(testStrings[0].c_str(), "  Test: divisible by %s", divisibleByString);
    test.testDivisibleBy = stoi(divisibleByString);

    char trueMonkeyIndex[16] = {};
    sscanf(testStrings[1].c_str(), "    If true: throw to monkey %ld", &test.monkeyIndexToThrowWhenTestIsTrue);

    char falseMonkeyIndex[16] = {};
    sscanf(testStrings[2].c_str(), "    If false: throw to monkey %ld", &test.monkeyIndexToThrowWhenTestIsFalse);

    return test;
}

Monkey GetMonkey(const vector<string>& monkeyInputLines)
{
    Monkey monkey;

    monkey.startingItemsWorry = GetStartingItemsWorry(monkeyInputLines[1]);
    monkey.operation = GetOperation(monkeyInputLines[2]);

    auto range = monkeyInputLines | views::drop(3);
    vector<string> testStrings(range.begin(), range.end());
    monkey.test = GetTest(testStrings);
    return monkey;
}

vector<string> GetFileContents(const string& fileName)
{
    ifstream fileStream(fileName, ios::in);
    vector<string> input;

    string line;
    while(getline(fileStream, line, '\n'))
        input.emplace_back(line);

    return input;
}

size_t GetPartOneAnswer(const vector<string>& inputLines)
{
    vector<Monkey> monkeys;
    auto range = inputLines | views::split("");
    for(auto subRange : range)
    {
        vector<string> monkeyStrings;
        for(auto str: subRange)
        {
            monkeyStrings.emplace_back(str);
        }

        monkeys.emplace_back(GetMonkey(monkeyStrings));
    }

    for(const size_t index: views::iota(0) | views::take(20))
    {
        for(Monkey& monkey: monkeys)
        {
            for(const size_t worry: monkey.startingItemsWorry)
            {
                const size_t increasedWorry = monkey.operation.Process(worry);
                const size_t reducedWorry = increasedWorry / 3;

                if((reducedWorry % monkey.test.testDivisibleBy) == 0)
                    monkeys[monkey.test.monkeyIndexToThrowWhenTestIsTrue].startingItemsWorry.push_back(reducedWorry);
                else
                    monkeys[monkey.test.monkeyIndexToThrowWhenTestIsFalse].startingItemsWorry.push_back(reducedWorry);

                ++monkey.itemsInspected;
            }
            monkey.startingItemsWorry.clear();
        }
    }

    size_t maxItemsInspected = 0;
    size_t secondMaxItemsInspected = 0;
    for(const Monkey& monkey: monkeys)
    {
        size_t itemsInspected = monkey.itemsInspected;
        if(itemsInspected > maxItemsInspected)
            swap(itemsInspected, maxItemsInspected);

        if(itemsInspected > secondMaxItemsInspected)
            swap(itemsInspected, secondMaxItemsInspected);
    }

    return maxItemsInspected * secondMaxItemsInspected;
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneAnswer(inputLines) << endl;

    return 0;
}
