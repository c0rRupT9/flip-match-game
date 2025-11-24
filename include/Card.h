#pragma once
#include <iostream>

template<typename T>
class Card {
private:
    T value;
    bool isMatched;
    bool isRevealed;

public:
    Card() : value(), isMatched(false), isRevealed(false) {}
    Card(T v) : value(v), isMatched(false), isRevealed(false) {}

    void reveal() { isRevealed = true; }
    void hide() { isRevealed = false; }
    void markMatched() { isMatched = true; }

    bool matched() const { return isMatched; }
    bool revealed() const { return isRevealed; }

    T getValue() const { return value; }

    bool operator==(const Card<T>& c) const {
        return value == c.value;
    }
};
