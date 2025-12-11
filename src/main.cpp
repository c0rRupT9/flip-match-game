#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <queue>
#include <set>
using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayGame(const vector<vector<string>> &board, const vector<vector<bool>> &revealed, const vector<vector<bool>> &removed, int score1, int score2, int currentPlayer)
{
    clearScreen();
    cout << "                   Welcome to\n";
    cout << "============ Memory Card Flip Game ============\n\n";
    if (currentPlayer == 1)
        cout << "                ->Player 1<-   Score: " << score1 << "\n";
    else
        cout << "                  Player 1     Score: " << score1 << "\n";
    cout << "\n        ";
    for (int c = 1; c <= 6; c++)
        cout << c << "    ";
    cout << "\n";
    for (int r = 0; r < 6; r++)
    {
        cout << "    " << (r + 1) << "   ";
        for (int c = 0; c < 6; c++)
        {
            if (removed[r][c])
                cout << "     ";
            else if (revealed[r][c])
                cout << "|" << board[r][c] << "| ";
            else
                cout << "|| ";
        }
        cout << "\n";
    }
    cout << "\n";
    if (currentPlayer == 2)
        cout << "                ->Player 2<-   Score: " << score2 << "\n";
    else
        cout << "                  Player 2     Score: " << score2 << "\n";
    cout << "\n";
}

int main()
{
    srand(time(0));
    queue<string> movesHistory;
    queue<int> playerTurn;
    playerTurn.push(1);
    playerTurn.push(2);
    set<pair<int,int>> pickedBefore;
    vector<string> leftChars = {"A","2","3","4","5","6","7","8","9","J","Q","K"};
    vector<string> rightChars = {"R", "B"};
    vector<string> symbols;
    int count = 0;
    for (int i = 0; i < (int)leftChars.size() && count < 18; i++)
    {
        for (int j = 0; j < 2 && count < 18; j++)
        {
            symbols.push_back(leftChars[i] + rightChars[j]);
            count++;
        }
    }
    vector<string> deck;
    for (int i = 0; i < 18; i++)
    {
        deck.push_back(symbols[i]);
        deck.push_back(symbols[i]);
    }
    for (int i = 0; i < 36; i++)
    {
        int j = rand() % 36;
        string temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
    vector<vector<string>> board(6, vector<string>(6));
    int idx = 0;
    for (int r = 0; r < 6; r++)
        for (int c = 0; c < 6; c++)
            board[r][c] = deck[idx++];
    vector<vector<bool>> revealed(6, vector<bool>(6, false));
    vector<vector<bool>> removed(6, vector<bool>(6, false));
    int score1 = 0, score2 = 0;
    for (int r = 0; r < 6; r++)
        for (int c = 0; c < 6; c++)
            revealed[r][c] = true;
    displayGame(board, revealed, removed, score1, score2, 1);
    cout << "All cards are revealed. Memorize them.\n";
    cout << "Press Enter when ready...";
    cin.ignore();
    cin.get();
    for (int r = 0; r < 6; r++)
        for (int c = 0; c < 6; c++)
            revealed[r][c] = false;
    int matchedPairs = 0;
    while (matchedPairs < 18)
    {
        int currentPlayer = playerTurn.front();
        displayGame(board, revealed, removed, score1, score2, currentPlayer);
        int r1, c1, r2, c2;
        cout << "Player " << currentPlayer << ", enter FIRST card (row col): ";
        cin >> r1 >> c1;
        r1--; c1--;
        while (r1 < 0 || r1 >= 6 || c1 < 0 || c1 >= 6 || removed[r1][c1] || false)
        {
            cout << "Invalid. Enter again: ";
            cin >> r1 >> c1;
            r1--; c1--;
        }
        revealed[r1][c1] = true;
        movesHistory.push("Player " + to_string(currentPlayer) + ": Flipped (" + to_string(r1+1) + "," + to_string(c1+1) + ") = " + board[r1][c1]);
        displayGame(board, revealed, removed, score1, score2, currentPlayer);
        cout << "Enter SECOND card (row col): ";
        cin >> r2 >> c2;
        r2--; c2--;
        while (r2 < 0 || r2 >= 6 || c2 < 0 || c2 >= 6 || removed[r2][c2] || (r1 == r2 && c1 == c2))
        {
            if (r1 == r2 && c1 == c2)
                cout << "Card has already been picked! Enter again: ";
            else
                cout << "Invalid. Enter again: ";
            cin >> r2 >> c2;
            r2--; c2--;
        }
        revealed[r2][c2] = true;
        movesHistory.push("Player " + to_string(currentPlayer) + ": Flipped (" + to_string(r2+1) + "," + to_string(c2+1) + ") = " + board[r2][c2]);
        displayGame(board, revealed, removed, score1, score2, currentPlayer);
        cout << "Press Enter...";
        cin.ignore();
        cin.get();
        if (board[r1][c1] == board[r2][c2])
        {
            cout << "MATCH!\n";
            removed[r1][c1] = true;
            removed[r2][c2] = true;
            matchedPairs++;
            pickedBefore.insert({r1, c1});
            pickedBefore.insert({r2, c2});
            if (currentPlayer == 1) score1++;
            else score2++;
            cout << "Press Enter...";
            cin.get();
        }
        else
        {
            cout << "Not a match.\n";
            revealed[r1][c1] = false;
            revealed[r2][c2] = false;
            cout << "Press Enter...";
            cin.get();
        }
        playerTurn.pop();
        playerTurn.push(currentPlayer);
        if (playerTurn.front() == 1)
        {
            char choice;
            cout << "End game early? (y/n): ";
            cin >> choice;
            if (choice == 'y' || choice == 'Y')
                break;
        }
    }
    displayGame(board, revealed, removed, score1, score2, playerTurn.front());
    if (score1 > score2)
        cout << "=========== Winner: Player 1! ===========\n\n";
    else if (score2 > score1)
        cout << "=========== Winner: Player 2! ===========\n\n";
    else
        cout << "=========== Draw! ===========\n\n";
    char choice;
    cout << "Would you like a summary of all player moves? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        cout << "\nMoves History:\n";
        while (!movesHistory.empty())
        {
            cout << movesHistory.front() << endl;
            movesHistory.pop();
        }
    }
    cout << "\nThanks for Playing!\n\n";
    return 0;
}