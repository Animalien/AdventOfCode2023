///////////////////////////
// Problem 5 - If you give a seed a fertilizer

#include "AdventOfCode2023.h"

#define myassert(arg)
//#define myassert(arg) assert(arg)

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

        Span() = default;
        Span(BigInt s) : start(s), num(-1), end(-1), offset(0) {}
        Span(BigInt dest, BigInt src, BigInt n) : start(src), num(n), end(start + num - 1), offset(dest - src)
        {
            myassert(start >= 0);
        }

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
            myassert(input >= 0);
            BigInt spanIndex = -1;
            if (lastInput == (input - 1))
            {
                spanIndex = lastSpanIndex;
                if ((spanList[lastSpanIndex].end > 0) && (input > spanList[lastSpanIndex].end))
                    ++spanIndex;
            }
            else
            {
                spanIndex = FindSpanIndexForInput(input);
            }

            lastInput = input;
            lastSpanIndex = spanIndex;

            const Span& span = spanList[spanIndex];
            return input + span.offset;
        }

        BigInt FindSpanIndexForInput(BigInt input) const
        {
            const auto b = std::upper_bound(spanList.begin(), spanList.end(), input);
            return b - spanList.begin() - 1;
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

        const bool printMaps = true;        //verbose;

        if (printMaps)
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

            if (printMaps)
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

            for (BigInt i = 1; i < (BigInt)newMap.spanList.size(); ++i)
            {
                const BigInt prevEnd = newMap.spanList[i - 1].end;
                const BigInt requiredStart = prevEnd + 1;
                const BigInt currStart = newMap.spanList[i].start;
                if (currStart > requiredStart)
                {
                    // we have a gap, so insert a 0-offset span between
                    Span newSpan;
                    newSpan.start = requiredStart;
                    newSpan.end = currStart - 1;
                    newSpan.num = newSpan.end - newSpan.start + 1;
                    newSpan.offset = 0;
                    newMap.spanList.insert(newMap.spanList.begin() + i, newSpan);
                    ++i;
                }
            }

            if (printMaps)
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

        // now MERGE all the maps

        Map mergedMap = mapMap["seed"];
        Map* mapToMerge = &(mapMap[mergedMap.dest]);
        for (;;)
        {
            Map newMergedMap;
            MergeMaps(mergedMap, *mapToMerge, newMergedMap);
            mergedMap = newMergedMap;

            if (mapToMerge->dest == "location")
                break;

            mapToMerge = &(mapMap[mapToMerge->dest]);
        }

        // now run each of the seeds through the merged map, and find the lowest result (Part 1)

        if (verbose)
            printf("\n  Seeds through Merged Map, Part 1:\n");

        BigInt lowestLocation = -1;
        for (BigInt seed: seeds)
        {
            const BigInt num = mergedMap.MapNumber(seed);

            if (verbose)
                printf("      Seed %lld goes through merged map, comes out %lld\n", seed, num);

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

        // now run each of the seeds through the merged map, and find the lowest result (Part 2)

        if (verbose)
            printf("\n  Seeds through Merged Map, Part 2:\n");

        lowestLocation = -1;
        for (BigInt seedListIndex = 0; seedListIndex < (BigInt)seeds.size(); seedListIndex += 2)
        {
            const BigInt seed0 = seeds[seedListIndex];
            const BigInt seed1 = seed0 + seeds[seedListIndex + 1];

            if (verbose)
                printf("    Seed range %lld to %lld:\n", seed0, seed1);

            for (BigInt seed = seed0; seed <= seed1; ++seed)
            {
                const BigInt num = mergedMap.MapNumber(seed);

                if (verbose)
                    printf("      Seed %lld goes through merged map, comes out %lld\n", seed, num);

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

    void MergeMaps(const Map& map1, const Map& map2, Map& newMap)
    {
        newMap = Map();

        BigInt map1SpanIndex = 0;
        BigInt span1Start = 0;

        for (;;)
        {
            const BigInt span2Start = map1.MapNumber(span1Start);
            myassert(span2Start >= 0);
            const BigInt map2SpanIndex = map2.FindSpanIndexForInput(span2Start);

            myassert((map1.spanList[map1SpanIndex].end < 0) || (map1.spanList[map1SpanIndex].end >= span1Start));
            const BigInt span1Remaining =
                (map1.spanList[map1SpanIndex].end >= 0) ? (map1.spanList[map1SpanIndex].end - span1Start + 1) : MAX_BIG_INT;

            myassert((map2.spanList[map2SpanIndex].end < 0) || (map2.spanList[map2SpanIndex].end >= span2Start));
            const BigInt span2Remaining =
                (map2.spanList[map2SpanIndex].end >= 0) ? (map2.spanList[map2SpanIndex].end - span2Start + 1) : MAX_BIG_INT;

            if (span1Remaining <= span2Remaining)
            {
                // last merged span bit from span 1

                Span newSpan;
                newSpan.start = span1Start;
                if (span1Remaining < MAX_BIG_INT)
                {
                    newSpan.num = span1Remaining;
                    newSpan.end = newSpan.start + newSpan.num - 1;
                }
                else
                {
                    newSpan.num = -1;
                    newSpan.end = -1;
                }
                newSpan.offset = map1.spanList[map1SpanIndex].offset + map2.spanList[map2SpanIndex].offset;
                newMap.spanList.push_back(newSpan);

                ++map1SpanIndex;
                if (map1SpanIndex >= (BigInt)map1.spanList.size())
                    break;

                span1Start = map1.spanList[map1SpanIndex].start;
            }
            else
            {
                // make a piece equal to this stretch from span 2

                Span newSpan;
                newSpan.start = span1Start;
                newSpan.num = span2Remaining;
                newSpan.end = newSpan.start + newSpan.num - 1;
                newSpan.offset = map1.spanList[map1SpanIndex].offset + map2.spanList[map2SpanIndex].offset;
                newMap.spanList.push_back(newSpan);

                myassert(newSpan.num >= 0);
                span1Start += newSpan.num;
            }
        }
    }
};

Problem5 problem5;
