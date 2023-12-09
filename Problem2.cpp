///////////////////////////
// Problem 2 - Cube Conundrum

#include "AdventOfCode2023.h"

class Problem2 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 2; }

    virtual void Run() override
    {
        RunOnData("Day2Example.txt", true);
        RunOnData("Day2Input.txt", false);
    }

private:
    void RunOnData(const char* filename, bool verbose)
    {
        printf("For file '%s'...\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        BigInt totalPossibleGameIDs = 0;
        BigInt totalPowerPossible = 0;
        ProcessGames(lines, 12, 13, 14, totalPossibleGameIDs, totalPowerPossible, verbose);

        printf("  Total IDs of possible games = %lld, total power possible = %lld\n\n", totalPossibleGameIDs, totalPowerPossible);
    }

    void ProcessGames(const StringList& lines, BigInt numRed, BigInt numGreen, BigInt numBlue, BigInt& totalPossibleGameIDs, BigInt& totalPowerPossible, bool verbose)
    {
        totalPossibleGameIDs = 0;
        totalPowerPossible = 0;
        for (const std::string& line: lines)
        {
            if (verbose)
                printf("    Line = '%s'\n", line.c_str());

            BigInt gameID = -1;
            bool isPossible = false;
            BigInt powerPossible = 0;
            ProcessGame(line, numRed, numGreen, numBlue, gameID, isPossible, powerPossible, verbose);

            if (isPossible)
                totalPossibleGameIDs += gameID;

            totalPowerPossible += powerPossible;

            if (verbose)
                printf("      gameID = %lld, is possible = %s, power possible = %lld\n", gameID, isPossible ? "yes" : "no", powerPossible);
        }
    }

    void ProcessGame(const std::string& line, BigInt numRed, BigInt numGreen, BigInt numBlue, BigInt& gameID, bool& isPossible, BigInt& powerPossible, bool verbose)
    {
        StringList tokens;
        Tokenize(line, tokens, ' ');
        gameID = strtoll(tokens[1].c_str(), nullptr, 10);

        isPossible = true;

        BigInt redMax = 0;
        BigInt greenMax = 0;
        BigInt blueMax = 0;

        for (BigInt tokenIndex = 2; tokenIndex < (BigInt)tokens.size(); tokenIndex += 2)
        {
            const BigInt number = strtoll(tokens[tokenIndex].c_str(), nullptr, 10);
            if (strncmp(tokens[tokenIndex + 1].c_str(), "red", 3) == 0)
            {
                if (isPossible && (number > numRed))
                {
                    if (verbose)
                        printf("      game is impossible because red %lld is > %lld\n", number, numRed);
                    isPossible = false;
                }

                if (number > redMax)
                    redMax = number;
            }
            else if (strncmp(tokens[tokenIndex + 1].c_str(), "green", 5) == 0)
            {
                if (isPossible && (number > numGreen))
                {
                    if (verbose)
                        printf("      game is impossible because green %lld is > %lld\n", number, numGreen);
                    isPossible = false;
                }

                if (number > greenMax)
                    greenMax = number;
            }
            else if (strncmp(tokens[tokenIndex + 1].c_str(), "blue", 4) == 0)
            {
                if (isPossible && (number > numBlue))
                {
                    if (verbose)
                        printf("      game is impossible because blue %lld is > %lld\n", number, numBlue);
                    isPossible = false;
                }

                if (number > blueMax)
                    blueMax = number;
            }
        }

        powerPossible = redMax * greenMax * blueMax;

        if (verbose)
            printf(
                "      red max %lld * green max %lld * blue max %lld = power possible %lld\n",
                redMax,
                greenMax,
                blueMax,
                powerPossible);
    }
};

Problem2 problem2;
