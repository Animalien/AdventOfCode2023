///////////////////////////
// Problem 4 - Scratchcards

#include "AdventOfCode2023.h"

class Problem4 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 4; }

    virtual void Run() override
    {
        RunOnData("Day4Example.txt", true);
        RunOnData("Day4Input.txt", false);
    }

private:
    void RunOnData(const char* filename, bool verbose)
    {
        printf("For file '%s'...\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        BigInt sumOfCardPoints = 0;
        for (const auto& line: lines)
        {
            const BigInt cardPoints = CalcCardPoints(line, verbose);
            sumOfCardPoints += cardPoints;
        }

        printf("  Sum of card points = %lld\n\n", sumOfCardPoints);
    }

    BigInt CalcCardPoints(const std::string& card, bool verbose)
    {
        StringList tokens;
        Tokenize(card, tokens, ' ');

        if (verbose)
            printf("    Card %s\n", tokens[1].c_str());

        BigIntUnorderedSet winningSet;

        BigInt tokenIndex = 2;
        for (; tokens[tokenIndex][0] != '|'; ++tokenIndex)
        {
            const BigInt winningNumber = strtoll(tokens[tokenIndex].c_str(), nullptr, 10);
            winningSet.insert(winningNumber);
        }

        ++tokenIndex;
        BigInt cardPoints = 0;
        for (; tokenIndex < (BigInt)tokens.size(); ++tokenIndex)
        {
            const BigInt cardNumber = strtoll(tokens[tokenIndex].c_str(), nullptr, 10);
            if (winningSet.count(cardNumber) > 0)
            {
                if (cardPoints > 0)
                    cardPoints += cardPoints;
                else
                    cardPoints = 1;
                if (verbose)
                    printf("      %lld is a winning number!  card points goes to %lld\n", cardNumber, cardPoints);
            }
        }

        return cardPoints;
    }
};

Problem4 problem4;
