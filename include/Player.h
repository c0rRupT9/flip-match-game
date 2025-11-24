#pragma once
#include <string>

class Player {
private:
    std::string name;
    int score;

public:
    Player(std::string n = "") : name(n), score(0) {}

    void addScore() { score++; }
    int getScore() const { return score; }
    std::string getName() const { return name; }
};
