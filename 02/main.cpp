#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <ranges>
#include <vector>

using namespace std;
namespace rv = ranges::views;

enum class Hand : int8_t
{
    Rock,
    Paper,
    Scissors
};

enum class GameStatus : int8_t
{
    Win,
    Lose,
    Draw
};

vector<string> GetFileContents(const string& fileName)
{
    ifstream fileStream(fileName, ios::in);

    vector<string> input;

    string line;
    while(getline(fileStream, line, '\n'))
        input.emplace_back(line);

    return input;
}

Hand GetElfHandFromChar(const char elfHandChar)
{
    switch(elfHandChar)
    {
        case 'A':
            return Hand::Rock;
        case 'B':
            return Hand::Paper;
        case 'C':
            return Hand::Scissors;
    }

    throw "Invalid elfHandChar";
    return {};
}

Hand GetMyHandFromChar(const char myHandChar)
{
    switch(myHandChar)
    {
        case 'X':
            return Hand::Rock;
        case 'Y':
            return Hand::Paper;
        case 'Z':
            return Hand::Scissors;
    }

    throw "Invalid myHandChar";
    return {};
}

GameStatus GetGameStatusFromChar(const char gameStatusChar)
{
    switch(gameStatusChar)
    {
        case 'X':
            return GameStatus::Lose;
        case 'Y':
            return GameStatus::Draw;
        case 'Z':
            return GameStatus::Win;
    }

    throw "Invalid gameStatusChar";
    return {};
}

pair<Hand, Hand> GetPartOneGameFromInputLine(const string& inputLine)
{
    stringstream inputStringStream(inputLine);
    char elfHandChar, myHandChar;
    inputStringStream >> elfHandChar >> myHandChar;

    return { GetElfHandFromChar(elfHandChar), GetMyHandFromChar(myHandChar) };
}

int operator<=>(const Hand hand1, const Hand hand2)
{
    if(hand1 == hand2)
        return 0;

    if(hand1 == Hand::Rock && hand2 == Hand::Paper)
        return -1;

    if(hand1 == Hand::Rock && hand2 == Hand::Scissors)
        return 1;

    if(hand1 == Hand::Paper && hand2 == Hand::Rock)
        return 1;

    if(hand1 == Hand::Paper && hand2 == Hand::Scissors)
        return -1;

    if(hand1 == Hand::Scissors && hand2 == Hand::Rock)
        return -1;

    if(hand1 == Hand::Scissors && hand2 == Hand::Paper)
        return 1;

    return {};
}

uint32_t GetScoreForMyHand(const Hand myHand)
{
    switch(myHand)
    {
        case Hand::Rock:
            return 1;
        case Hand::Paper:
            return 2;
        case Hand::Scissors:
            return 3;
    }

    throw "Invalid myHand";
    return {};
}

uint32_t GetPartOneScoreForGame(const pair<Hand, Hand>& game)
{
    uint32_t score = GetScoreForMyHand(game.second);

    auto winner = game.first <=> game.second;
    if(winner == 0)
        score += 3;
    else if(winner > 0)
        score += 0;
    else // if(winner < 0)
        score += 6;

    return score;
}

uint32_t GetPartOneScore(const vector<string>& inputLines)
{
    auto scoresRange =
        inputLines
        | rv::transform(GetPartOneGameFromInputLine)
        | rv::transform(GetPartOneScoreForGame)
        ;

    return accumulate(scoresRange.begin(), scoresRange.end(), 0);
}

pair<Hand, GameStatus> GetPartTwoGameFromInputLine(const string& inputLine)
{
    stringstream inputStringStream(inputLine);
    char elfHandChar, gameStatusChar;
    inputStringStream >> elfHandChar >> gameStatusChar;

    return { GetElfHandFromChar(elfHandChar), GetGameStatusFromChar(gameStatusChar) };
}

constexpr uint32_t GetScoreForGameStatus(const GameStatus gameStatus)
{
    switch(gameStatus)
    {
        case GameStatus::Draw:
            return 3;
        case GameStatus::Win:
            return 6;
        case GameStatus::Lose:
            return 0;
    }

    throw "Invalid gameStatus";
    return {};
}

uint32_t GetScoreForHandToPlay(const pair<Hand, GameStatus>& game)
{
    const Hand& elfHand = game.first;
    const GameStatus& gameStatus = game.second;

    if(elfHand == Hand::Rock)
    {
        if(gameStatus == GameStatus::Draw)
            return GetScoreForMyHand(Hand::Rock);
        if(gameStatus == GameStatus::Win)
            return GetScoreForMyHand(Hand::Paper);
        if(gameStatus == GameStatus::Lose)
            return GetScoreForMyHand(Hand::Scissors);
    }

    if(elfHand == Hand::Paper)
    {
        if(gameStatus == GameStatus::Draw)
            return GetScoreForMyHand(Hand::Paper);
        if(gameStatus == GameStatus::Win)
            return GetScoreForMyHand(Hand::Scissors);
        if(gameStatus == GameStatus::Lose)
            return GetScoreForMyHand(Hand::Rock);
    }

    if(elfHand == Hand::Scissors)
    {
        if(gameStatus == GameStatus::Draw)
            return GetScoreForMyHand(Hand::Scissors);
        if(gameStatus == GameStatus::Win)
            return GetScoreForMyHand(Hand::Rock);
        if(gameStatus == GameStatus::Lose)
            return GetScoreForMyHand(Hand::Paper);
    }

    throw "Invalid state in GetScoreForHandToPlay";
    return {};
}

uint32_t GetScoreForPartTwoGame(const pair<Hand, GameStatus>& game)
{
    return GetScoreForGameStatus(game.second) + GetScoreForHandToPlay(game);
}

uint32_t GetPartTwoScore(const vector<string>& inputLines)
{
    auto scoresRange =
        inputLines
        | rv::transform(GetPartTwoGameFromInputLine)
        | rv::transform(GetScoreForPartTwoGame)
        ;

    return accumulate(scoresRange.begin(), scoresRange.end(), 0);
}

int main()
{
    vector<string> inputLines = GetFileContents("input");

    cout << "Part 1: " << GetPartOneScore(inputLines) << endl;
    cout << "Part 2: " << GetPartTwoScore(inputLines) << endl;

    return 0;
}