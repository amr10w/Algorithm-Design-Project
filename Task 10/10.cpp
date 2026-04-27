#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

// --- SHARED HELPER ---
// Flips 'H' to 'T' or 'T' to 'H' for neighbors
void flip(string &s, int idx) {
    if (idx >= 0 && idx < s.length() && s[idx] != '_') {
        s[idx] = (s[idx] == 'H') ? 'T' : 'H';
    }
}

// --- ALGORITHM 1: BACKTRACKING ---
// Tries every possible 'H' until all are removed
bool backtrack(string current, vector<int> &moves) {
    bool allRemoved = true;
    for (char c : current) if (c != '_') { allRemoved = false; break; }
    if (allRemoved) return true;

    for (int i = 0; i < current.length(); i++) {
        if (current[i] == 'H') {
            string nextState = current;
            nextState[i] = '_';
            flip(nextState, i - 1);
            flip(nextState, i + 1);
            
            moves.push_back(i);
            if (backtrack(nextState, moves)) return true;
            moves.pop_back(); // Undo move (backtrack)
        }
    }
    return false;
}

// --- ALGORITHM 2: EDGE-PICKING ---
// Always picks the right-most 'H' available
vector<int> edgePicking(string current) {
    vector<int> moves;
    int n = current.length();
    while (true) {
        int lastH = -1;
        for (int i = n - 1; i >= 0; i--) {
            if (current[i] == 'H') { lastH = i; break; }
        }
        if (lastH == -1) break; // No more H left

        current[lastH] = '_';
        flip(current, lastH - 1);
        flip(current, lastH + 1);
        moves.push_back(lastH);
    }
    for (char c : current) {
        if (c == 'T') {
            cout << "Edge-Picking failed (T remains)!";
            return{}; 
        }
    }    
    return moves;
}

// --- TEST RUNNER ---
void runTest(string input) {
    cout << "TEST CASE: " << input << endl;
    cout << "------------------------------------" << endl;

    // Run Backtracking
    vector<int> btMoves;
    auto start1 = high_resolution_clock::now();
    bool solvedBT = backtrack(input, btMoves);
    auto stop1 = high_resolution_clock::now();
    auto timeBT = duration_cast<microseconds>(stop1 - start1);

    cout << "[Backtracking] ";
    if (solvedBT) {
        cout << "Time: " << timeBT.count() << "us | Moves: ";
        for (int m : btMoves) cout << m << " ";
    } else {
        cout << "No solution found.";
    }
    cout << endl;

    // Run Edge-Picking
    auto start2 = high_resolution_clock::now();
    vector<int> epMoves = edgePicking(input);
    auto stop2 = high_resolution_clock::now();
    auto timeEP = duration_cast<microseconds>(stop2 - start2);

    cout << "[Edge-Picking] ";
    cout << "Time: " << timeEP.count() << "us | Moves: ";
    for (int m : epMoves) cout << m << " ";
    cout << "\n\n";
}

int main() {
    // Case 1: Original input
    runTest("HTHHHTHHHTT");

    // Case 2: Short input
    runTest("HHT");

    // Case 3: Pattern that might be tricky
    runTest("THTH");

    return 0;
}