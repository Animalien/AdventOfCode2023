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
    static const char sc_emptyChar = '.';
    static const char sc_gearChar = '*';

    typedef std::unordered_map<BigInt, BigIntList> GearMap;

    void RunOnData(const char* filename, bool verbose)
    {
        printf("For file '%s'...\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        BigInt sumOfPartNumbers = 0;
        BigInt sumOfGearRatios = 0;
        ProcessSchematic(lines, sumOfPartNumbers, sumOfGearRatios, verbose);

        printf("  Sum of part numbers = %lld, sum of gear ratios = %lld\n", sumOfPartNumbers, sumOfGearRatios);
    }

    void ProcessSchematic(const StringList& lines, BigInt& sumOfPartNumbers, BigInt& sumOfGearRatios, bool verbose)
    {
        sumOfPartNumbers = 0;

        BigInt x = 0;
        BigInt y = 0;

        GearMap gearMap;

        for (;;)
        {
            BigInt partNumber = -1;
            FindNextPart(lines, gearMap, x, y, partNumber, verbose);

            if (partNumber < 0)
                break;

            sumOfPartNumbers += partNumber;
            if (verbose)
                printf("    Found part number %lld, sum = %lld\n", partNumber, sumOfPartNumbers);
        }

        if (verbose)
            printf("    Found gears:\n");

        sumOfGearRatios = 0;
        for (const auto& gearMapPair: gearMap)
        {
            const BigInt gearX = gearMapPair.first % lines.size();
            const BigInt gearY = gearMapPair.first / lines.size();
            if (gearMapPair.second.size() == 2)
            {
                const BigInt gearFirstPartNumber = gearMapPair.second[0];
                const BigInt gearSecondPartNumber = gearMapPair.second[1];
                const BigInt gearRatio = gearFirstPartNumber * gearSecondPartNumber;
                sumOfGearRatios += gearRatio;

                if (verbose)
                    printf(
                        "      Found '%c' char, is a gear, gear ratio = %lld * %lld = %lld, sum of gear ratios = %lld\n",
                        (int)sc_gearChar,
                        gearFirstPartNumber,
                        gearSecondPartNumber,
                        gearRatio,
                        sumOfGearRatios);
            }
            else
            {
                if (verbose)
                    printf(
                        "      Found '%c' char, is NOT a gear, because it has %lld parts adjacent\n",
                        (int)sc_gearChar,
                        gearMapPair.second.size());
            }
        }
    }

    void FindNextPart(
        const StringList& lines, GearMap& gearMap,
        BigInt& x,
        BigInt& y,
        BigInt& partNumber,
        bool verbose)
    {
        static BigIntList foundGearCharList;
        foundGearCharList.clear();

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
            bool isAPart = SchematicCharIsASymbol(lines, foundGearCharList, x - 1, y - 1)
                           || SchematicCharIsASymbol(lines, foundGearCharList, x - 1, y)
                           || SchematicCharIsASymbol(lines, foundGearCharList, x - 1, y + 1);
            for (;;)
            {
                const char ch = GetSchematicChar(lines, x, y);
                isAPart = isAPart || SchematicCharIsASymbol(lines, foundGearCharList, x, y - 1)
                          || SchematicCharIsASymbol(lines, foundGearCharList, x, y)
                          || SchematicCharIsASymbol(lines, foundGearCharList, x, y + 1);

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
            {
                for (BigInt gearIndex: foundGearCharList)
                {
                    gearMap[gearIndex].push_back(partNumber);
                }
                return;
            }

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

    bool SchematicCharIsASymbol(const StringList& lines, BigIntList& foundGearCharList, BigInt x, BigInt y)
    {
        const char ch = GetSchematicChar(lines, x, y);
        if (ch == sc_gearChar)
            foundGearCharList.push_back(y * (BigInt)lines.size() + x);
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
