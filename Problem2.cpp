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

        const BigInt total =
            CountTotalIDsOfPossibleGames(lines, 12, 13, 14, verbose);

        printf("  Total IDs of possible games = %lld\n\n", total);
    }

    BigInt CountTotalIDsOfPossibleGames(const StringList& lines, BigInt numRed, BigInt numGreen, BigInt numBlue, bool verbose)
    {
        BigInt total = 0;
        for (const std::string& line: lines)
        {
            BigInt gameID = -1;
            const bool isPossible = IsGamePossible(line, numRed, numGreen, numBlue, gameID, verbose);

            if (isPossible)
                total += gameID;

            if (verbose)
                printf("    Line = '%s', is possible = %s, gameID = %lld\n", line.c_str(), isPossible ? "yes" : "no", gameID);
        }

        return total;
    }

    bool IsGamePossible(const std::string& line, BigInt numRed, BigInt numGreen, BigInt numBlue, BigInt& gameID, bool verbose)
    {
        StringList tokens;
        Tokenize(line, tokens, ' ');
        gameID = strtoll(tokens[1].c_str(), nullptr, 10);

        for (BigInt tokenIndex = 2; tokenIndex < (BigInt)tokens.size(); tokenIndex += 2)
        {
            const BigInt number = strtoll(tokens[tokenIndex].c_str(), nullptr, 10);
            if (strncmp(tokens[tokenIndex + 1].c_str(), "red", 3) == 0)
            {
                if (number > numRed)
                {
                    if (verbose)
                        printf("      red %lld is > %lld\n", number, numRed);
                    return false;
                }
            }
            else if (strncmp(tokens[tokenIndex + 1].c_str(), "green", 5) == 0)
            {
                if (number > numGreen)
                {
                    if (verbose)
                        printf("      green %lld is > %lld\n", number, numGreen);
                    return false;
                }
            }
            else if (strncmp(tokens[tokenIndex + 1].c_str(), "blue", 4) == 0)
            {
                if (number > numBlue)
                {
                    if (verbose)
                        printf("      blue %lld is > %lld\n", number, numBlue);
                    return false;
                }
            }
        }

        return true;
    }
};

Problem2 problem2;
