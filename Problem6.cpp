///////////////////////////
// Problem 6 - Name

#include "AdventOfCode2023.h"

class Problem6 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 6; }

    virtual void Run() override
    {
        RunOnData("Day6Example.txt", true);
        RunOnData("Day6Input.txt", false);
    }

private:
    void RunOnData(const char* filename, bool verbose)
    {
        printf("For file '%s'...\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        StringList timeTokens;
        Tokenize(lines[0], timeTokens, ' ');
        StringList distTokens;
        Tokenize(lines[1], distTokens, ' ');

        // Part 1

        BigInt numWaysWinProduct = 1;
        for (BigInt tokenIndex = 1; tokenIndex < (BigInt)timeTokens.size(); ++tokenIndex)
        {
            const BigInt time = strtoll(timeTokens[tokenIndex].c_str(), nullptr, 10);
            const BigInt distRecord = strtoll(distTokens[tokenIndex].c_str(), nullptr, 10);

            if (verbose)
                printf("    Time = %lld, Dist record = %lld\n", time, distRecord);

            const BigInt numWaysWin = CalcNumWaysWin(time, distRecord, verbose);

            if (verbose)
                printf("      Num ways to win = %lld\n", numWaysWin);

            numWaysWinProduct *= numWaysWin;
        }

        printf("    Product of numbers of ways to win = %lld (Part 1)\n", numWaysWinProduct);

        // Part 2

        std::string timeConcat;
        std::string distConcat;
        for (BigInt i = 1; i < (BigInt)timeTokens.size(); ++i)
        {
            timeConcat += timeTokens[i];
            distConcat += distTokens[i];
        }

        const BigInt time = strtoll(timeConcat.c_str(), nullptr, 10);
        const BigInt distRecord = strtoll(distConcat.c_str(), nullptr, 10);

        printf("    Time = %lld, Dist record = %lld\n", time, distRecord);

        const BigInt numWaysWin = CalcNumWaysWin(time, distRecord, false /*verbose*/);

        printf("    Num ways to win = %lld (Part 2)\n", numWaysWin);
    }

    BigInt CalcNumWaysWin(BigInt time, BigInt distRecord, bool verbose)
    {
        BigInt numWaysWin = 0;
        for (BigInt hold = 1; hold < time; ++hold)
        {
            const BigInt speed = hold;
            const BigInt run = time - hold;
            const BigInt dist = speed * run;

            if (verbose)
                printf(
                    "      Holding for %lld, causes moving at speed %lld for %lld time, moving a total of %lld distance, ",
                    hold,
                    speed,
                    run,
                    dist);

            if (dist > distRecord)
            {
                ++numWaysWin;

                if (verbose)
                    printf("which is a new record, and contributes to numWaysWin = %lld\n", numWaysWin);
            }
            else
            {
                if (verbose)
                    printf("which is NOT a new record\n");
            }
        }
        return numWaysWin;
    }
};

Problem6 problem6;
