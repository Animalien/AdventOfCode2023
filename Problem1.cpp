///////////////////////////
// Problem 1 - Trebuchet

#include "AdventOfCode2023.h"

class Problem1 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 1; }

    virtual void Run() override
    {
        printf("Test, Part 1:\n");
        RunProblem("Day1Example.txt", false, true);

        printf("\n\nTest, Part 2:\n");
        RunProblem("Day1Example2.txt", true, true);

        printf("\n\nInput, Part 1:\n");
        RunProblem("Day1Input.txt", false, false);

        printf("\n\nInput, Part 2:\n");
        RunProblem("Day1Input.txt", true, false);
    }

private:
    const StringList m_digitNamesList = { "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

    void RunProblem(const char* fileName, bool partTwo, bool verbose)
    {
        StringList lines;
        ReadFileLines(fileName, lines);

        BigInt total = 0;
        for (const std::string& line: lines)
        {
            BigInt firstDigit = 0;
            for (BigInt i = 0; i < (BigInt)line.length(); ++i)
            {
                const char ch = line[i];
                if (std::isdigit(ch))
                {
                    firstDigit = (BigInt)(int)(ch - '0');
                    break;
                }
                else if (partTwo && LineContainsNamedDigit(line, i, +1, firstDigit))
                {
                    break;
                }
            }

            BigInt secondDigit = 0;
            for (BigInt i = line.length() - 1; i >= 0; --i)
            {
                const char ch = line[i];
                if (std::isdigit(ch))
                {
                    secondDigit = (BigInt)(int)(ch - '0');
                    break;
                }
                else if (partTwo && LineContainsNamedDigit(line, i, -1, secondDigit))
                {
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

    bool LineContainsNamedDigit(const std::string& line, BigInt startPos, BigInt direction, BigInt& digit)
    {
        bool nameMatchSoFar[] = { true, true, true, true, true, true, true, true, true, true };
        BigInt lineCharIndex = startPos;
        BigInt nameCharIndex = 0;
        for (;;)
        {
            const char lineChar = line[lineCharIndex];

            bool haveOneMatchingSoFar = false;
            for (BigInt currDigit = 1; currDigit < 10; ++currDigit)
            {
                if (!nameMatchSoFar[currDigit])
                    continue;

                const std::string& digitName = m_digitNamesList[currDigit];
                if (nameCharIndex < (BigInt)digitName.length())
                {
                    const char nameChar = (direction > 0) ? digitName[nameCharIndex] : digitName[digitName.length() - 1 - nameCharIndex];
                    if (nameChar == lineChar)
                    {
                        if (nameCharIndex == (digitName.length() - 1))
                        {
                            digit = currDigit;
                            return true;
                        }

                        haveOneMatchingSoFar = true;
                    }
                    else
                    {
                        nameMatchSoFar[currDigit] = false;
                    }
                }
            }

            if (!haveOneMatchingSoFar)
                return false;

            lineCharIndex += direction;
            if ((lineCharIndex < 0) || (lineCharIndex >= (BigInt)line.length()))
                return false;

            ++nameCharIndex;
        }
    }
};

Problem1 problem1;
