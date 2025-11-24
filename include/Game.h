#pragma once
#include "Board.h"
#include "Player.h"

class Game {
protected:
    Board board;
    Player p1, p2;
    int currentPlayer;

public:
    Game(int r, int c);
    virtual void start();     // virtual function
    virtual void turn();      // virtual function
    void switchPlayer();
};
