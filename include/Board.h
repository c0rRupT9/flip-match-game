#pragma once
#include "Card.h"
#include <vector>

class Board {
private:
    int rows, cols;
    std::vector<std::vector<Card<char>>> grid;

public:
    Board(int r, int c);

    void shuffle();
    void revealAll();
    void hideAll();
    void display();
    Card<char>& getCard(int r, int c);
};
