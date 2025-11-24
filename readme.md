## 📁 Project Structure

```
flip-match-game/
│
├── docs/
│   └── Proposal.pdf
│
├── include/
│   ├── Board.h
│   ├── Card.h
│   ├── Game.h
│   └── Player.h
│
├── src/
│   ├── board.cpp
│   ├── card.cpp
│   ├── game.cpp
│   └── player.cpp
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

## Project Description

### **Header Files (`include/`)**
Contain all class declarations, method signatures, and enumerations. They define how different components of the game interact.

### **Source Files (`src/`)**
Contain function and class definitions.  
One `.cpp` file will later be designated as the **main game engine** once all modules are complete.

---

## Game Structure

### **1. Cards**
- Cards are generated with random values.  
- Each card is represented as:  
  `std::pair<int, char>`  
  - `int` → number (2–10, or 0 for J/Q/K)  
  - `char` → suit (`A`, `S`, `H`, `D`)
- Total cards = `(rows × cols) / 2`, and the other half are duplicates.
- Cards are stored in a `std::vector` or `unordered_set` (implementation still being finalized).

### **2. Board**
`board.cpp` uses `Card.h` to create and store card objects.  
Cards are randomly shuffled and placed on the board grid.

### **3. Player**
`player.cpp` handles:
- score  
- time  
- turns  
- player statistics  

### **4. Game**
`game.cpp` manages:
- gameplay loop  
- input handling  
- validating revealed cards  
- checking win conditions  
- formatting and printing the board  

---

## How to Play

Clone the repository:

```bash
git clone https://github.com/c0rRupT9/flip-match-game.git
```

Navigate into the project:

```bash
cd flip-match-game/src
```

Compile:

```bash
g++ main.cpp board.cpp card.cpp game.cpp player.cpp -o flipgame.exe
```

Run:

```bash
./flipgame.exe
```
Or for command prompt:
```cmd
flipgame
```

---
