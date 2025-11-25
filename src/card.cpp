#include "Card.h"
#include <iostream>
#include <random>
using namespace std;

    char number[] = {'2', '3', '4' ,'5' ,'6' ,'7' ,'8' ,'9' ,'0', 'K', 'Q', 'J' };
    char decks[] = {'S', 'H', 'D', 'A'};
class Card {
private:

    std::pair<char, char> card;
    bool isMatched;
    bool isRevealed;

public:
    Card()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        int numCount = sizeof(number) / sizeof(number[0]);
        int deckCount = sizeof(decks) / sizeof(decks[0]);
        std::uniform_int_distribution<int> distNum(0, numCount - 1);
        std::uniform_int_distribution<int> distDeck(0, deckCount - 1);

        int number_index = distNum(gen);
        int deck_index = distDeck(gen);

        card.first = number[number_index];
        card.second = decks[deck_index];

    }

};