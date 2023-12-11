///////////////////////////
// Problem 7 - Camel Cards

#include "AdventOfCode2023.h"

class Problem7 : public ProblemBase
{
public:
    virtual int GetProblemNum() const override { return 7; }

    virtual void Run() override
    {
        RunOnData("Day7Example.txt", true);
        RunOnData("Day7Input.txt", false);
    }

private:
    static const BigInt sc_handSize = 5;

    enum CardType
    {
        N2,
        N3,
        N4,
        N5,
        N6,
        N7,
        N8,
        N9,
        N10,
        Jack,
        Queen,
        King,
        Ace
    };
    typedef std::unordered_map<char, CardType> CardTypeMap;
    static const CardTypeMap sc_cardTypeMap;

    enum HandType
    {
        HighCard,
        OnePair,
        TwoPair,
        ThreeOfAKind,
        FullHouse,
        FourOfAKind,
        FiveOfAKind,
    };
    typedef std::unordered_map<HandType, std::string> HandTypeNameMap;
    static const HandTypeNameMap sc_handTypeNameMap;

    struct Hand
    {
        std::string handString;
        CardType cardList[sc_handSize];
        HandType type = HighCard;
        BigInt tieBreaker = 0;
        BigInt bid = 0;
        BigInt rank = 0;
        BigInt winnings = 0;

        bool operator>(const Hand& rhs) const
        {
            if (type > rhs.type)
                return true;
            if (type < rhs.type)
                return false;

            return tieBreaker > rhs.tieBreaker;
        }
    };
    typedef std::vector<Hand> HandList;

    void RunOnData(const char* filename, bool verbose)
    {
        printf("For file '%s'...\n", filename);

        StringList lines;
        ReadFileLines(filename, lines);

        if (verbose)
            printf("  Hands:\n");

        HandList handList;
        handList.reserve(lines.size());
        StringList tokens;
        for (const std::string& line: lines)
        {
            Tokenize(line, tokens, ' ');

            Hand hand;
            hand.handString = tokens[0];
            hand.type = DetermineHandType(hand.handString, hand.cardList);
            hand.tieBreaker = CalcHandTieBreaker(hand.cardList);
            hand.bid = strtoll(tokens[1].c_str(), nullptr, 10);

            handList.push_back(hand);
        }

        std::sort(handList.begin(), handList.end(), std::greater<>());

        BigInt totalWinnings = 0;
        for (BigInt handIndex = 0; handIndex < (BigInt)handList.size(); ++handIndex)
        {
            Hand& hand = handList[handIndex];
            hand.rank = (BigInt)handList.size() - handIndex;
            hand.winnings = hand.bid * hand.rank;

            totalWinnings += hand.winnings;

            if (verbose)
                printf(
                    "    Hand = %s, type = %s, tieBreaker = %lld, bid = %lld, rank = %lld, winnings = %lld\n",
                    hand.handString.c_str(),
                    sc_handTypeNameMap.find(hand.type)->second.c_str(),
                    hand.tieBreaker,
                    hand.bid,
                    hand.rank,
                    hand.winnings);
        }

        printf("  Total winnings = %lld\n", totalWinnings);
    }

    HandType DetermineHandType(const std::string& handString, CardType cardList[sc_handSize])
    {
        std::unordered_map<CardType, int> cardCountMap;
        for (int i = 0; i < sc_handSize; ++i)
        {
            cardList[i] = sc_cardTypeMap.find(handString[i])->second;
            ++(cardCountMap[cardList[i]]);
        }

        static std::string cardCountString;
        cardCountString.reserve(sc_handSize);
        cardCountString.clear();
        for (auto mapPair: cardCountMap)
        {
            cardCountString += ('0' + (char)mapPair.second);
        }
        std::sort(cardCountString.begin(), cardCountString.end(), std::greater<>());

        if (cardCountString == "5")
            return FiveOfAKind;
        if (cardCountString == "41")
            return FourOfAKind;
        if (cardCountString == "32")
            return FullHouse;
        if (cardCountString == "311")
            return ThreeOfAKind;
        if (cardCountString == "221")
            return TwoPair;
        if (cardCountString == "2111")
            return OnePair;

        return HighCard;
    }

    BigInt CalcHandTieBreaker(CardType cardList[sc_handSize])
    {
        BigInt tieBreaker = 0;
        for (int i = 0; i < sc_handSize; ++i)
        {
            tieBreaker <<= 4;
            tieBreaker |= (BigInt)cardList[i];
        }
        return tieBreaker;
    }
};

const Problem7::CardTypeMap Problem7::sc_cardTypeMap = { { '2', N2 },    { '3', N3 },   { '4', N4 }, { '5', N5 },  { '6', N6 },
                                                         { '7', N7 },    { '8', N8 },   { '9', N9 }, { 'T', N10 }, { 'J', Jack },
                                                         { 'Q', Queen }, { 'K', King }, { 'A', Ace } };
const Problem7::HandTypeNameMap Problem7::sc_handTypeNameMap = {
    { HighCard, "High Card" },           { OnePair, "One Pair" },     { TwoPair, "Two Pair" },
    { ThreeOfAKind, "Three Of A Kind" }, { FullHouse, "Full House" }, { FourOfAKind, "Four Of A Kind" },
    { FiveOfAKind, "Five Of A Kind" }
};


Problem7 problem7;
