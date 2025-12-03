// player.cpp
// Player struct with score and simple helpers.

#include <string>
#include <chrono>

#ifndef PLAYER_CPP_INCLUDED
#define PLAYER_CPP_INCLUDED

struct Player {
    std::string name;
    int score = 0;
    // For extensibility, we can track total time used etc.
    std::chrono::seconds time_left{10}; // per-turn default stored as convenience (not persisted across turns)
    Player() = default;
    Player(const std::string &n): name(n) {}
};

#endif // PLAYER_CPP_INCLUDED
