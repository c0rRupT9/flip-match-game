// card.cpp
// Provides CardItem generation and simple Card struct used by other modules.

#include <string>
#include <vector>
#include <unordered_set>
#include <random>
#include <algorithm>

#ifndef CARD_CPP_INCLUDED
#define CARD_CPP_INCLUDED

struct Card {
    std::string id;     // unique id like "A_R" or "10_B"
    bool faceUp = false;
    bool matched = false;
};

// Generate an unordered_set of unique card identifiers (value + color) and return as vector.
// Values: A,2-9,J,Q,K  Colors: R,B
inline std::vector<std::string> generate_unique_items(int needed = 18) {
    std::vector<std::string> ranks = {"A","2","3","4","5","6","7","8","9","J","Q","K"};
    std::vector<std::string> colors = {"R","B"}; // per spec

    std::unordered_set<std::string> set;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Create all possible combos first then shuffle and pick needed
    std::vector<std::string> all;
    for (auto &r: ranks) {
        for (auto &c: colors) {
            all.push_back(r + "_" + c);
        }
    }

    std::shuffle(all.begin(), all.end(), gen);

    for (size_t i = 0; i < all.size() && (int)set.size() < needed; ++i) {
        set.insert(all[i]);
    }

    // move to vector
    std::vector<std::string> out;
    out.reserve(set.size());
    for (auto &s: set) out.push_back(s);
    return out;
}

// create deck: take unique items vector (size N), duplicate each (so 2N cards), shuffle and return vector<Card>
inline std::vector<Card> build_deck_from_unique(const std::vector<std::string>& uniqueItems) {
    std::vector<Card> deck;
    deck.reserve(uniqueItems.size() * 2);
    for (auto &id: uniqueItems) {
        Card c1{ id, false, false };
        Card c2{ id, false, false };
        deck.push_back(c1);
        deck.push_back(c2);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(deck.begin(), deck.end(), gen);
    return deck;
}

// small helper to pretty-print face-up card as two-line strings for display
inline std::pair<std::string, std::string> draw_faceup(const Card &c) {
    // id is like "2_R" -> split on underscore
    auto pos = c.id.find('_');
    std::string left = c.id.substr(0, pos);
    std::string right = (pos == std::string::npos ? "" : c.id.substr(pos+1));
    // left padded to 2 chars, right padded to 2
    std::string line1 = "|";
    if (left.size() == 1) line1 += left + " ";
    else line1 += left;
    line1 += "|";

    std::string line2 = "| ";
    if (!right.empty()) line2 += right;
    else line2 += " ";
    line2 += "|";
    return {line1, line2};
}

#endif // CARD_CPP_INCLUDED
