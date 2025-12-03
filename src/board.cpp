// board.cpp
// Creates the 6x6 board and provides functions to access/format the grid.

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#ifndef BOARD_CPP_INCLUDED
#define BOARD_CPP_INCLUDED

// Depends on Card struct from card.cpp — we include via forward reference in compilation.
extern std::vector<Card> build_deck_from_unique(const std::vector<std::string>&);
extern std::vector<std::string> generate_unique_items(int);

// A Board holds 36 Card objects arranged row-major 6x6.
struct Board {
    static constexpr int ROWS = 6;
    static constexpr int COLS = 6;
    std::vector<Card> cells; // size 36

    Board() {
        // create 18 unique items and build deck (duplicated and shuffled)
        auto unique = generate_unique_items(18);
        cells = build_deck_from_unique(unique);
        if ((int)cells.size() != ROWS * COLS) {
            // safety: if fewer, fill with duplicates of first
            while ((int)cells.size() < ROWS * COLS) {
                Card c = cells[0];
                cells.push_back(c);
            }
        }
    }

    // index helpers
    inline int index(int r, int c) const { return r * COLS + c; }

    // flip card at coordinates (0-based). Returns false if invalid or already matched.
    bool reveal(int r, int c) {
        if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return false;
        Card &cd = cells[index(r,c)];
        if (cd.matched || cd.faceUp) return false;
        cd.faceUp = true;
        return true;
    }

    void hide(int r, int c) {
        if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return;
        Card &cd = cells[index(r,c)];
        if (!cd.matched) cd.faceUp = false;
    }

    // mark matched pair
    void set_matched(int r1,int c1,int r2,int c2) {
        cells[index(r1,c1)].matched = true;
        cells[index(r2,c2)].matched = true;
        cells[index(r1,c1)].faceUp = true;
        cells[index(r2,c2)].faceUp = true;
    }

    // get card id at location
    std::string id_at(int r, int c) const {
        return cells[index(r,c)].id;
    }

    bool is_faceup(int r,int c) const { return cells[index(r,c)].faceUp; }
    bool is_matched(int r,int c) const { return cells[index(r,c)].matched; }

    bool all_matched() const {
        for (auto &c: cells) if (!c.matched) return false;
        return true;
    }

    // render the board as ASCII with 2-line per card style
    // returns vector of strings representing lines to print
    std::vector<std::string> render_lines() const {
        std::vector<std::string> out;
        // top border row for column headers
        // We'll print rows each with two lines per card, separated by spaces.
        for (int r = 0; r < ROWS; ++r) {
            std::string line1, line2;
            for (int c = 0; c < COLS; ++c) {
                const Card &cd = cells[index(r,c)];
                if (cd.faceUp || cd.matched) {
                    auto p = draw_faceup(cd);
                    line1 += p.first;
                    line2 += p.second;
                } else {
                    line1 += "|**|";
                    line2 += "|**|";
                }
                if (c < COLS-1) {
                    line1 += " ";
                    line2 += " ";
                }
            }
            out.push_back(line1);
            out.push_back(line2);
        }
        return out;
    }
};

#endif // BOARD_CPP_INCLUDED
