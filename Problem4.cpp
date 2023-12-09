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
        BigIntList numCardsList;
        numCardsList.resize(lines.size(), 1);
        for (BigInt cardIndex = 0; cardIndex<(BigInt)lines.size(); ++cardIndex)
        {
            BigInt cardPoints = 0;
            ProcessCard(lines, cardIndex, cardPoints, numCardsList, verbose);
            sumOfCardPoints += cardPoints;
        }

        BigInt totalNumCards = 0;
        for (BigInt numCards: numCardsList)
        {
            totalNumCards += numCards;
        }

        printf("  Sum of card points (Part 1) = %lld, total num cards (Part 2) = %lld\n\n", sumOfCardPoints, totalNumCards);
    }

    void ProcessCard(const StringList& cardList, BigInt cardIndex, BigInt& cardPoints, BigIntList& numCardsList, bool verbose)
    {
        StringList tokens;
        Tokenize(cardList[cardIndex], tokens, ' ');

        if (verbose)
            printf("    Card %lld\n", cardIndex + 1);

        BigIntUnorderedSet winningSet;

        BigInt tokenIndex = 2;
        for (; tokens[tokenIndex][0] != '|'; ++tokenIndex)
        {
            const BigInt winningNumber = strtoll(tokens[tokenIndex].c_str(), nullptr, 10);
            winningSet.insert(winningNumber);
        }

        ++tokenIndex;
        BigInt numWinningNumbers = 0;
        cardPoints = 0;
        for (; tokenIndex < (BigInt)tokens.size(); ++tokenIndex)
        {
            const BigInt cardNumber = strtoll(tokens[tokenIndex].c_str(), nullptr, 10);
            if (winningSet.count(cardNumber) > 0)
            {
                ++numWinningNumbers;
                if (cardPoints > 0)
                    cardPoints += cardPoints;
                else
                    cardPoints = 1;
                if (verbose)
                    printf("      %lld is a winning number!  card points goes to %lld\n", cardNumber, cardPoints);
            }
        }

        const BigInt numberOfThisCardCopies = numCardsList[cardIndex];

        if (verbose)
            printf(
                "      Card has %lld winning numbers, so next %lld cards get %lld more copies (because this card already has %lld copies):\n",
                numWinningNumbers,
                numWinningNumbers,
                numberOfThisCardCopies,
                numberOfThisCardCopies);

        for (BigInt i = 0; i < numWinningNumbers; ++i)
        {
            const BigInt nextCardIndex = cardIndex + 1 + i;
            if (nextCardIndex >= (BigInt)cardList.size())
                break;

            numCardsList[nextCardIndex] += numberOfThisCardCopies;

            if (verbose)
                printf(
                    "        Card %lld gets %lld more copies, to a total of %lld\n",
                    nextCardIndex + 1,
                    numberOfThisCardCopies,
                    numCardsList[nextCardIndex]);
        }
    }
};

Problem4 problem4;
