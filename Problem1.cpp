///////////////////////////
// Problem 1 - Trebuchet

#include "AdventOfCode2023.h"

class Problem1 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 1; }

    virtual void Run() override
    {
        printf("Test:\n");
        RunProblem("Day1Example.txt", true);

        printf("\n\nInput:\n");
        RunProblem("Day1Input.txt", false);
    }

private:
    void RunProblem(const char* fileName, bool verbose)
    {
        StringList lines;
        ReadFileLines(fileName, lines);

        BigInt total = 0;
        for (const std::string& line: lines)
        {
            BigInt firstDigit = 0;
            for (auto iter = line.cbegin(); iter != line.cend(); ++iter)
            {
                const char ch = *iter;
                if (std::isdigit(ch))
                {
                    firstDigit = (BigInt)(int)(ch - '0');
                    break;
                }
            }

            BigInt secondDigit = 0;
            for (auto iter = line.crbegin(); iter != line.crend(); ++iter)
            {
                const char ch = *iter;
                if (std::isdigit(ch))
                {
                    secondDigit = (BigInt)(int)(ch - '0');
                    break;
                }
            }

            BigInt number = firstDigit * 10 + secondDigit;
            total += number;

            if (verbose)
                printf("  line = '%s', number = %lld, total = %lld\n", line.c_str(), number, total);
        }

        printf("Total = %lld\n", total);
    }
};

Problem1 problem1;
