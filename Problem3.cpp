///////////////////////////
// Problem 3 - Gear Ratios

#include "AdventOfCode2023.h"

class Problem3 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 3; }

    virtual void Run() override
    {
        RunOnData("Day3Example.txt", true);
        RunOnData("Day3Input.txt", false);
    }

private:
    void RunOnData(const char* filename, bool verbose)
    {
        printf("For file '%s'...\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        BigInt sumOfPartNumbers = 0;
        ProcessSchematic(lines, sumOfPartNumbers, verbose);

        printf("  Sum of part numbers = %lld\n", sumOfPartNumbers);
    }

    void ProcessSchematic(const StringList& lines, BigInt& sumOfPartNumbers, bool verbose)
    {
        sumOfPartNumbers = 0;

        BigInt x = 0;
        BigInt y = 0;

        for (;;)
        {
            BigInt partNumber = -1;
            FindNextPart(lines, x, y, partNumber, verbose);

            if (partNumber < 0)
                return;

            sumOfPartNumbers += partNumber;
            if (verbose)
                printf("    Found part number %lld, sum = %lld\n", partNumber, sumOfPartNumbers);
        }
    }

    void FindNextPart(
        const StringList& lines,
        BigInt& x,
        BigInt& y,
        BigInt& partNumber,
        bool verbose)
    {
        for (;;)
        {
            // first look for the next number

            for (;;)
            {
                const char ch = GetSchematicChar(lines, x, y);
                if (std::isdigit(ch))
                    break;

                if (!StepXAndY(lines, x, y))
                {
                    partNumber = -1;
                    return;
                }
            }

            // now trace along the digits, accruing the part number, but also checking to see if there are any symbols near the number, making this a genuine part

            partNumber = 0;
            bool isAPart = SchematicCharIsASymbol(lines, x - 1, y - 1) || SchematicCharIsASymbol(lines, x - 1, y)
                           || SchematicCharIsASymbol(lines, x - 1, y + 1);
            for (;;)
            {
                const char ch = GetSchematicChar(lines, x, y);
                isAPart = isAPart || SchematicCharIsASymbol(lines, x, y - 1) || SchematicCharIsASymbol(lines, x, y)
                          || SchematicCharIsASymbol(lines, x, y + 1);

                if (std::isdigit(ch))
                {
                    partNumber *= 10;
                    partNumber += (BigInt)(int)(ch - '0');
                }
                else
                {
                    break;
                }

                ++x;
            }

            if (verbose)
                printf("      Found a number %lld, is a part = %s\n", partNumber, isAPart ? "yes" : "no");
            if (isAPart)
                return;

            partNumber = -1;
        }
    }

    bool StepXAndY(const StringList& lines, BigInt& x, BigInt& y)
    {
        if (y >= (BigInt)lines.size())
            return false;

        ++x;
        if (x >= (BigInt)lines[0].length())
        {
            x = 0;
            ++y;
            if (y >= (BigInt)lines.size())
                return false;
        }

        return true;
    }

    static const char sc_emptyChar = '.';

    bool SchematicCharIsASymbol(const StringList& lines, BigInt x, BigInt y)
    {
        const char ch = GetSchematicChar(lines, x, y);
        return ((ch != sc_emptyChar) && !std::isdigit(ch));
    }

    char GetSchematicChar(const StringList& lines, BigInt x, BigInt y)
    {
        if (y < 0)
            return sc_emptyChar;
        if (y >= (BigInt)lines.size())
            return sc_emptyChar;

        const std::string& line = lines[y];

        if (x < 0)
            return sc_emptyChar;
        if (x >= (BigInt)line.length())
            return sc_emptyChar;

        return line[x];
    }

    BigInt ParsePartNumber(const StringList& lines, BigInt x1, BigInt x2, BigInt y)
    {
        BigInt number = 0;
        const std::string& line = lines[y];
        for (BigInt x = x1; x <= x2; ++x)
        {
            const BigInt digit = (BigInt)(int)(line[x] - '0');
            number *= 10;
            number += digit;
        }
        return number;
    }
};

Problem3 problem3;
