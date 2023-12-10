///////////////////////////
// Problem 5 - If you give a seed a fertilizer

#include "AdventOfCode2023.h"

class Problem5 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 5; }

    virtual void Run() override
    {
        RunOnData("Day5Example.txt", true);
        RunOnData("Day5Input.txt", false);
    }

private:
    struct Span
    {
        BigInt start = -1;
        BigInt num = -1;
        BigInt end = -1;
        BigInt offset = 0;

        Span(BigInt s) : start(s), num(-1), end(-1), offset(0) {}
        Span(BigInt dest, BigInt src, BigInt n) : start(src), num(n), end(start + num - 1), offset(dest - src) {}

        operator BigInt() const { return start; }
    };
    typedef std::vector<Span> SpanList;

    struct Map
    {
        std::string source;
        std::string dest;
        Map* destPtr = nullptr;
        std::vector<Span> spanList;

        mutable BigInt lastInput = -1000;
        mutable BigInt lastSpanIndex = -1;

        BigInt MapNumber(BigInt input) const
        {
            BigInt spanIndex = -1;
            if (lastInput == (input - 1))
            {
                spanIndex = lastSpanIndex;
                if ((spanList[lastSpanIndex].end > 0) && (input > spanList[lastSpanIndex].end))
                    ++spanIndex;
            }
            else
            {
                const auto b = std::upper_bound(spanList.begin(), spanList.end(), input);
                spanIndex = b - spanList.begin() - 1;
            }

            lastInput = input;
            lastSpanIndex = spanIndex;

            const Span& span = spanList[spanIndex];
            return input + span.offset;
        }
    };
    typedef std::unordered_map<std::string, Map> MapMap;
    
    void RunOnData(const char* filename, bool verbose)
    {
        printf("For file '%s'...\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        // parse the seeds

        StringList tokens;
        Tokenize(lines[0], tokens, ' ');

        if (verbose)
            printf("  Seeds: ");

        BigIntList seeds;
        for (BigInt i = 1; i < (BigInt)tokens.size(); ++i)
        {
            const BigInt seedNum = strtoll(tokens[i].c_str(), nullptr, 10);
            seeds.push_back(seedNum);

            if (verbose)
                printf("%lld ", seedNum);
        }

        // parse the maps

        if (verbose)
            printf("\n  Maps:\n");

        MapMap mapMap;
        StringList mapNameTokens;
        BigInt lineIndex = 2;
        while (lineIndex < (BigInt)lines.size())
        {
            Tokenize(lines[lineIndex], tokens, ' ');
            Tokenize(tokens[0].c_str(), mapNameTokens, '-');

            Map newMap;
            newMap.source = mapNameTokens[0];
            newMap.dest = mapNameTokens[2];

            if (verbose)
                printf("    Map %s to %s:\n", newMap.source.c_str(), newMap.dest.c_str());

            ++lineIndex;
            while ((lineIndex < (BigInt)lines.size()) && !lines[lineIndex].empty())
            {
                Tokenize(lines[lineIndex], tokens, ' ');
                newMap.spanList.push_back(Span(
                    strtoll(tokens[0].c_str(), nullptr, 10),
                    strtoll(tokens[1].c_str(), nullptr, 10),
                    strtoll(tokens[2].c_str(), nullptr, 10)));
                ++lineIndex;
            }

            std::sort(newMap.spanList.begin(), newMap.spanList.end());

            if (newMap.spanList[0].start > 0)
            {
                Span preSpan(0, 0, newMap.spanList.front().start);
                newMap.spanList.insert(newMap.spanList.begin(), preSpan);
            }

            const BigInt postSpanStart = newMap.spanList.back().end + 1;
            Span postSpan(postSpanStart);
            newMap.spanList.insert(newMap.spanList.end(), postSpan);

            if (verbose)
            {
                for (const Span& span: newMap.spanList)
                {
                    if (span.num > 0)
                        printf(
                            "      Span %lld to %lld, offset %lld\n",
                            span.start, span.end, span.offset);
                    else
                        printf("      Span %lld to infinity\n", span.start);
                }
            }

            mapMap[newMap.source] = newMap;
            ++lineIndex;
        }

        // now run each of the seeds through the maps, and find the lowest result (Part 1)

        if (verbose)
            printf("\n  Seeds through Maps, Part 1:\n");

        Map*const seedMap = &(mapMap["seed"]);
        Map* finalMap = nullptr;

        BigInt lowestLocation = -1;
        for (BigInt seed: seeds)
        {
            if (verbose)
                printf("    Seed %lld:\n", seed);

            BigInt num = seed;
            Map* map = seedMap;
            for (;;)
            {
                if (verbose)
                    printf("      Num %lld goes through map %s-to-%s, comes out ", num, map->source.c_str(), map->dest.c_str());

                num = map->MapNumber(num);

                if (verbose)
                    printf("%lld\n", num);

                if (finalMap)
                {
                    if (map == finalMap)
                        break;
                }
                else
                {
                    if (map->dest == "location")
                    {
                        finalMap = map;
                        break;
                    }
                }

                if (!map->destPtr)
                    map->destPtr = &(mapMap[map->dest]);

                map = map->destPtr;
            }

            if ((lowestLocation < 0) || (num < lowestLocation))
            {
                lowestLocation = num;

                if (verbose)
                    printf("      Lowest location updated to %lld\n", lowestLocation);
            }
            else
            {
                if (verbose)
                    printf("      Location is not lowest\n");
            }
        }

        printf("  Lowest location found = %lld\n", lowestLocation);

        // now run each of the seeds through the maps, and find the lowest result (Part 1)

        if (verbose)
            printf("\n  Seeds through Maps, Part 2:\n");

        lowestLocation = -1;
        for (BigInt seedListIndex = 0; seedListIndex < (BigInt)seeds.size(); seedListIndex += 2)
        {
            const BigInt seed0 = seeds[seedListIndex];
            const BigInt seed1 = seed0 + seeds[seedListIndex + 1];

            if (verbose)
                printf("    Seed range %lld to %lld:\n", seed0, seed1);

            for (BigInt seed = seed0; seed <= seed1; ++seed)
            {
                if (verbose)
                    printf("      Seed %lld:\n", seed);

                BigInt num = seed;
                Map* map = seedMap;
                for (;;)
                {
                    if (verbose)
                        printf(
                            "        Num %lld goes through map %s-to-%s, comes out ", num, map->source.c_str(), map->dest.c_str());

                    num = map->MapNumber(num);

                    if (verbose)
                        printf("%lld\n", num);

                    if (finalMap)
                    {
                        if (map == finalMap)
                            break;
                    }
                    else
                    {
                        if (map->dest == "location")
                        {
                            finalMap = map;
                            break;
                        }
                    }

                    if (!map->destPtr)
                        map->destPtr = &(mapMap[map->dest]);

                    map = map->destPtr;
                }

                if ((lowestLocation < 0) || (num < lowestLocation))
                {
                    lowestLocation = num;

                    if (verbose)
                        printf("        Lowest location updated to %lld\n", lowestLocation);
                }
                else
                {
                    if (verbose)
                        printf("        Location is not lowest\n");
                }
            }
        }

        printf("  Lowest location found = %lld\n", lowestLocation);
    }
};

Problem5 problem5;
