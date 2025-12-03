// game.cpp
// Implements the gameplay loop, printing, input-with-timeout, and matching logic.

#include <iostream>
#include <string>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <sstream>
#include <iomanip>

#ifndef GAME_CPP_INCLUDED
#define GAME_CPP_INCLUDED

extern struct Board;
extern struct Player;

// helper: read a single line from stdin with timeout (seconds). Returns true and fills out if input received before timeout.
// Uses a separate thread and condition_variable.
inline bool get_line_with_timeout(std::string &out, int timeout_seconds) {
    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;
    std::string local;
    std::thread reader([&](){
        std::string s;
        if (!std::getline(std::cin, s)) {
            // EOF or error
        } else {
            {
                std::unique_lock<std::mutex> lk(mtx);
                local = s;
                done = true;
            }
            cv.notify_one();
        }
    });

    {
        std::unique_lock<std::mutex> lk(mtx);
        if (!cv.wait_for(lk, std::chrono::seconds(timeout_seconds), [&](){ return done; })) {
            // timeout
            // attempt to detach/stop reader: reader likely blocked on getline; we can't forcibly kill thread
            // To keep behavior safe, we'll return false and leave thread to finish when user types something (it won't block program termination)
            // On some systems this can cause resources to linger; acceptable for a console student project.
            // Note: You could use platform-specific non-blocking input for cleaner behavior.
            // We'll signal by not joining and detach the reader.
            reader.detach();
            return false;
        } else {
            out = local;
        }
    }

    if (reader.joinable()) reader.join();
    return true;
}

// Game controller
struct Game {
    Board board;
    Player p1;
    Player p2;
    int activePlayer = 1; // 1 or 2
    const int perTurnSeconds = 10;
    const int revealSeconds = 5;

    Game(): p1("Player 1"), p2("Player 2") {}

    void print_ui(int timer_remaining) {
        // Clear screen — best-effort
        #if defined(_WIN32) || defined(_WIN64)
            system("cls");
        #else
            std::cout << "\x1B[2J\x1B[H"; // ANSI clear screen
        #endif

        std::cout << "Memory Card Flip Game\n\n";
        std::cout << "Timer: " << timer_remaining << "s\n\n";
        std::cout << "Player 1 Score: " << p1.score << "\n\n";

        if (activePlayer == 1) std::cout << "->Player 1<-\n\n";
        else std::cout << " Player 1 \n\n";

        // board
        auto lines = board.render_lines();
        for (auto &ln : lines) {
            std::cout << ln << "\n";
        }
        std::cout << "\n";

        if (activePlayer == 2) std::cout << "->Player 2<-\n\n";
        else std::cout << " Player 2 \n\n";

        std::cout << "Player 2 Score: " << p2.score << "\n\n";
        std::cout << "Enter coordinates as R C (row and col) e.g. 1 1 for top-left. Rows and cols range 1-6.\n";
    }

    // parse coordinates input like "r c"
    bool parse_coords(const std::string &s, int &r, int &c) {
        std::istringstream iss(s);
        if (!(iss >> r >> c)) return false;
        r -= 1; c -= 1;
        if (r < 0 || r >= Board::ROWS || c < 0 || c >= Board::COLS) return false;
        return true;
    }

    void run() {
        while (!board.all_matched()) {
            int timer = perTurnSeconds;
            print_ui(timer);
            // first choice
            std::cout << "\nPlayer " << activePlayer << " - Choose first card (you have " << timer << "s)\n> " << std::flush;
            std::string input;
            bool got = get_line_with_timeout(input, timer);
            if (!got) {
                // timeout; pass turn
                std::cout << "\nTime up! No card selected. Passing turn...\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
                toggle_player();
                continue;
            }

            int r1,c1;
            if (!parse_coords(input, r1,c1)) {
                std::cout << "Invalid input. Passing turn.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                toggle_player();
                continue;
            }
            // reveal first
            if (!board.reveal(r1,c1)) {
                std::cout << "Can't reveal that card (already face-up/matched). Passing turn.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                toggle_player();
                continue;
            }

            // Update timer remaining by measuring elapsed time since prompt (approx). For simplicity we reduce full second
            // We'll give the remaining timer as per-turnSeconds - 1 (approx)
            timer = perTurnSeconds; // reset for second choice start
            print_ui(timer);
            std::cout << "\nFirst card selected at (" << r1+1 << "," << c1+1 << "). It will stay revealed for " << revealSeconds << "s.\n";
            // show for revealSeconds and then allow second pick (but per rules, reveal for ~5 seconds and then cards flipped? User said cards revealed for around 5 seconds then flipped and players then take turns. I interpret revealSeconds as immediate visual reveal before continuing to pick.)
            std::this_thread::sleep_for(std::chrono::seconds(revealSeconds));

            // After revealSeconds, show board (first card still faceUp). Prompt for second card with timeout perTurnSeconds.
            print_ui(timer);
            std::cout << "\nPlayer " << activePlayer << " - Choose second card (you have " << timer << "s)\n> " << std::flush;
            std::string input2;
            bool got2 = get_line_with_timeout(input2, timer);
            if (!got2) {
                // timeout: flip back the first card and pass turn
                std::cout << "\nTime up! Second card not selected. First card will flip back. Passing turn...\n";
                board.hide(r1,c1);
                std::this_thread::sleep_for(std::chrono::seconds(2));
                toggle_player();
                continue;
            }

            int r2,c2;
            if (!parse_coords(input2,r2,c2)) {
                std::cout << "Invalid input for second card. First card will flip back. Passing turn.\n";
                board.hide(r1,c1);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                toggle_player();
                continue;
            }

            if (r1 == r2 && c1 == c2) {
                std::cout << "Selected same card twice. First card will flip back. Passing turn.\n";
                board.hide(r1,c1);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                toggle_player();
                continue;
            }

            if (!board.reveal(r2,c2)) {
                std::cout << "Can't reveal that second card (already face-up/matched). First card will flip back. Passing turn.\n";
                board.hide(r1,c1);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                toggle_player();
                continue;
            }

            // Show both for revealSeconds then check match
            print_ui(timer);
            std::this_thread::sleep_for(std::chrono::seconds(revealSeconds));

            std::string id1 = board.id_at(r1,c1);
            std::string id2 = board.id_at(r2,c2);
            if (id1 == id2) {
                // matched
                board.set_matched(r1,c1,r2,c2);
                if (activePlayer == 1) p1.score++;
                else p2.score++;
                std::cout << "\nMatch! Player " << activePlayer << " gains 1 point.\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
                // per your rules, the other player will have their turn (so switch)
                toggle_player();
            } else {
                // not matched -> flip back
                board.hide(r1,c1);
                board.hide(r2,c2);
                std::cout << "\nNot a match. Cards flip back. Passing turn...\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
                toggle_player();
            }
        } // end while

        // Game over
        print_ui(0);
        std::cout << "\nAll cards matched!\n";
        std::cout << "Player 1 Score: " << p1.score << "\n";
        std::cout << "Player 2 Score: " << p2.score << "\n";
        if (p1.score > p2.score) std::cout << "Winner: Player 1\n";
        else if (p2.score > p1.score) std::cout << "Winner: Player 2\n";
        else std::cout << "Draw\n";
    }

    void toggle_player() {
        activePlayer = (activePlayer == 1) ? 2 : 1;
    }
};

#endif // GAME_CPP_INCLUDED
