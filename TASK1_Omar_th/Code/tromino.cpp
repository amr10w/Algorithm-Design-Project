#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <iomanip>
using namespace std;

#define MAX 128

int board[MAX][MAX];
int trominoId = 0;

struct Tromino {
    pair<int,int> cells[3];
    vector<int> neighbors;
    int color;
};

vector<Tromino> trominoes;

// ============================================
// PART 1: DIVIDE AND CONQUER - TILING
// ============================================
void tileBoard(int size, int topRow, int topCol,
               int missingRow, int missingCol) {
    
    if (size == 2) {
        Tromino t;
        int cellIndex = 0;
        for (int i = topRow; i < topRow + 2; i++) {
            for (int j = topCol; j < topCol + 2; j++) {
                if (board[i][j] == 0) {
                    t.cells[cellIndex++] = {i, j};
                    board[i][j] = trominoId + 1;
                }
            }
        }
        t.color = -1;
        trominoes.push_back(t);
        trominoId++;
        return;
    }

    int half = size / 2;
    int midRow = topRow + half;
    int midCol = topCol + half;

    int currentId = trominoId + 1;
    trominoId++;

    Tromino center;
    int cellIndex = 0;
    center.color = -1;

    if (!(missingRow < midRow && missingCol < midCol)) {
        board[midRow - 1][midCol - 1] = currentId;
        center.cells[cellIndex++] = {midRow - 1, midCol - 1};
    }
    if (!(missingRow < midRow && missingCol >= midCol)) {
        board[midRow - 1][midCol] = currentId;
        center.cells[cellIndex++] = {midRow - 1, midCol};
    }
    if (!(missingRow >= midRow && missingCol < midCol)) {
        board[midRow][midCol - 1] = currentId;
        center.cells[cellIndex++] = {midRow, midCol - 1};
    }
    if (!(missingRow >= midRow && missingCol >= midCol)) {
        board[midRow][midCol] = currentId;
        center.cells[cellIndex++] = {midRow, midCol};
    }

    trominoes.push_back(center);

    // TOP-LEFT
    if (missingRow < midRow && missingCol < midCol)
        tileBoard(half, topRow, topCol, missingRow, missingCol);
    else
        tileBoard(half, topRow, topCol, midRow - 1, midCol - 1);

    // TOP-RIGHT
    if (missingRow < midRow && missingCol >= midCol)
        tileBoard(half, topRow, midCol, missingRow, missingCol);
    else
        tileBoard(half, topRow, midCol, midRow - 1, midCol);

    // BOTTOM-LEFT
    if (missingRow >= midRow && missingCol < midCol)
        tileBoard(half, midRow, topCol, missingRow, missingCol);
    else
        tileBoard(half, midRow, topCol, midRow, midCol - 1);

    // BOTTOM-RIGHT
    if (missingRow >= midRow && missingCol >= midCol)
        tileBoard(half, midRow, midCol, missingRow, missingCol);
    else
        tileBoard(half, midRow, midCol, midRow, midCol);
}

// ============================================
// PART 2: BUILD NEIGHBOR LIST
// ============================================
void buildNeighborList() {
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    int n = (int)trominoes.size();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            bool found = false;
            for (int ci = 0; ci < 3 && !found; ci++) {
                for (int cj = 0; cj < 3 && !found; cj++) {
                    int r1 = trominoes[i].cells[ci].first;
                    int c1 = trominoes[i].cells[ci].second;
                    int r2 = trominoes[j].cells[cj].first;
                    int c2 = trominoes[j].cells[cj].second;

                    for (int d = 0; d < 4; d++) {
                        if (r1 + dx[d] == r2 && c1 + dy[d] == c2) {
                            trominoes[i].neighbors.push_back(j);
                            trominoes[j].neighbors.push_back(i);
                            found = true;
                            break;
                        }
                    }
                }
            }
        }
    }
}

// ============================================
// PART 3: BACKTRACKING - COLORING
// ============================================
bool isSafe(int trominoIndex, int color) {
    for (int neighborIndex : trominoes[trominoIndex].neighbors) {
        if (trominoes[neighborIndex].color == color) {
            return false;
        }
    }
    return true;
}

bool colorTrominoes(int index) {
    if (index == (int)trominoes.size()) {
        return true;
    }

    for (int c = 0; c < 3; c++) {
        if (isSafe(index, c)) {
            trominoes[index].color = c;

            if (colorTrominoes(index + 1))
                return true;

            trominoes[index].color = -1;
        }
    }

    return false;
}

// ============================================
// PRINT THE BOARD
// ============================================
void printBoard(int size) {
    string colors[] = {"RED", "BLUE", "GREEN"};

    cout << "\n=== BOARD (Tromino IDs) ===" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == -1)
                cout << setw(4) << "X";
            else
                cout << setw(4) << board[i][j];
        }
        cout << endl;
    }

    cout << "\n=== TROMINO COLORS ===" << endl;
    for (int i = 0; i < (int)trominoes.size(); i++) {
        cout << "Tromino " << i << ": " << colors[trominoes[i].color];
        cout << " | Cells: ";
        for (int c = 0; c < 3; c++) {
            cout << "(" << trominoes[i].cells[c].first 
                 << "," << trominoes[i].cells[c].second << ") ";
        }
        cout << endl;
    }

    cout << "\n=== COLORED BOARD ===" << endl;
    string colorBoard[MAX][MAX];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            colorBoard[i][j] = "X";

    for (int i = 0; i < (int)trominoes.size(); i++) {
        string c = colors[trominoes[i].color].substr(0, 1);
        for (int k = 0; k < 3; k++) {
            int r = trominoes[i].cells[k].first;
            int col = trominoes[i].cells[k].second;
            colorBoard[r][col] = c;
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << setw(4) << colorBoard[i][j];
        }
        cout << endl;
    }
}

// ============================================
// MAIN (with input validation)
// ============================================
int main() {
    int n;
    cout << "Enter n (board will be 2^n x 2^n): ";
    cin >> n;

    // INPUT VALIDATION: n range
    if (n <= 1 || n > 7) {
        cout << "Error: n must be between 2 and 7 "
             << "(board sizes 4x4 to 128x128)." << endl;
        return 1;
    }

    int size = 1 << n;

    int missRow, missCol;
    cout << "Enter missing square row (0 to " << size - 1 << "): ";
    cin >> missRow;
    cout << "Enter missing square col (0 to " << size - 1 << "): ";
    cin >> missCol;

    // INPUT VALIDATION: bounds
    if (missRow < 0 || missRow >= size || missCol < 0 || missCol >= size) {
        cout << "Error: Missing square position out of bounds." << endl;
        return 1;
    }

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = 0;

    board[missRow][missCol] = -1;

    cout << "\nTiling the board..." << endl;
    tileBoard(size, 0, 0, missRow, missCol);
    cout << "Tiling complete! Placed " << trominoes.size() 
         << " trominoes." << endl;

    cout << "Building neighbor list..." << endl;
    buildNeighborList();

    cout << "Coloring trominoes..." << endl;
    if (colorTrominoes(0)) {
        cout << "Coloring successful!" << endl;
        printBoard(size);
    } else {
        cout << "No valid coloring found!" << endl;
    }

    return 0;
}