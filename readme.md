## 📁 Project Structure

```
flip-match-game/
│
├── docs/
│   └── Proposal.pdf
├── src/
│   ├── main.cpp
│
├── .gitignore
├── .gitattributes
└── README.md
```

---

### `.gitignore`
Ignores unwanted executables (`.exe`) and object files (`.o`), build folders, and other temporary files.

### `.gitattributes`
Prevents Git from modifying line endings for text files and ensures consistency across Windows/Linux.

---
## 📝 Project Description

This project implements a terminal memory flip game using a 6×6 grid containing 36 shuffled cards (18 matching pairs).
Players take turns revealing two hidden cards; matched pairs increase their score and are removed from the board.

The program makes heavy use of STL containers:

`vector<vector<string>>` → Game board

`vector<vector<bool>>` → Revealed & removed states

`stack<string>` → Move history

`queue<int>` → Alternating player turns

`set<pair<int,int>>` → Track previously matched coordinates

## 🧩 Game Structure
### **1. Card System**

Cards are represented as 2-character strings such as "AR", "5B", "7R".

The left character comes from:

``A 2 3 4 5 6 7 8 9 J Q K``


The right character comes from the suits:

``R B``


Only `18` unique symbols are used to create 18 matching pairs.

These `36` cards are shuffled and placed into a 6×6 grid.

### **2. Board**

The board is stored as:

`vector<vector<string>> board(6, vector<string>(6));`


Supporting arrays:

`revealed[6][6]` → Whether a card is temporarily visible

`removed[6][6]` → Whether the card has been matched & removed

All cards are briefly revealed at the start to allow memorization.

### **3. Player System**

Players are managed using:

`queue<int> playerTurn`;


Player data tracked:

`Score (score1, score2)`

Current turn rotation

Matched positions `(inside a set)`

The queue rotates each turn so control alternates naturally.

### **4. Game Engine**

All logic runs inside main.cpp including:

Initial deck generation

Random shuffle

Input validation (row/col, re-selection prevention)

Match checking

Score updates

Turn switching

Display updates

Early end option

Move history is recorded with:

`stack<string> movesHistory;`

## 🎮 How to Play
Clone the repository

```bash
git clone https://github.com/c0rRupT9/flip-match-game.git
```
Navigate to the project
```bash
cd flip-match-game/src
```
```bash
Compile the game
```
(Windows MinGW / Linux / macOS)

```bash
g++ main.cpp -o flipgame
```
Run the game
```bash
./flipgame
```

Windows CMD:
```bash
flipgame
```

## ▶️ Gameplay Flow

Game reveals all cards for memorization.

`Player 1` begins and selects a card `(row col)`.

Selects second card.

If both cards match:

Pair removed

Score increased

Player goes again? → No, turns always alternate after each attempt

If not matched:

Cards flip back

Turn rotates

Game ends when:

All 18 pairs are matched

OR Player 1 manually ends early

Winner is announced based on total score.

## STL Containers Used
Container	Purpose
`vector<vector<string>>`	6×6 board storage

`vector<vector<bool>>`	Revealed & removed card states

`stack<string>`	Move history log

`queue<int>`	Handles player turns

`set<pair<int,int>>`	Tracks matched cards

`rand()` + `srand()`	Deck shuffling

##  Future Improvements

Save/load gameplay

AI Player mode

GUI version (SFML/SDL)

Sound effects

Difficulty levels