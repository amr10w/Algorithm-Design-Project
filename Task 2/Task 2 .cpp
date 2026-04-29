#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

const int n = 8;

vector<vector<int>> board(n, vector<int>(n, -1));

vector<pair<int, int>> knight_moves = {
    {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
    {1, -2}, {1, 2}, {2, -1}, {2, 1}
};

bool is_valid(int x, int y) {
    return (x >= 0 && x < n && y >= 0 && y < n);
}

vector<pair<int, int>> valid_moves(int x, int y) {
    vector<pair<int, int>> moves;
    for (auto [dx, dy] : knight_moves) {
        int mx = x + dx, my = y + dy;
        if (is_valid(mx, my) && board[mx][my] == -1) {
            moves.push_back({mx, my});
        }
    }
    return moves;
}

int count_onward_moves(int x, int y) {
    int degree = 0;
    for (auto [dx, dy] : knight_moves) {
        int nx = x + dx, ny = y + dy;
        if (is_valid(nx, ny) && board[nx][ny] == -1) {
            degree++;
        }
    }
    return degree;
}

int main() {
    // Random start
    srand(time(0));
    int start_x = rand() % n;
    int start_y = rand() % n;

    board[start_x][start_y] = 0;

    int x = start_x, y = start_y;

    for (int step = 1; step < n * n; step++) {
        vector<pair<int, pair<int, int>>> degree_of_candidates;

        vector<pair<int, int>> moves = valid_moves(x, y);

        for (auto [nx, ny] : moves) {
            int deg = count_onward_moves(nx, ny);
            degree_of_candidates.push_back({deg, {nx, ny}});
        }

        if (degree_of_candidates.empty()) {
            break;
        }

        // Shuffle to mimic Python randomness
        random_shuffle(degree_of_candidates.begin(), degree_of_candidates.end());

        // Sort by degree (Warnsdorff rule)
        sort(degree_of_candidates.begin(), degree_of_candidates.end());

        x = degree_of_candidates[0].second.first;
        y = degree_of_candidates[0].second.second;

        board[x][y] = step;
    }

    bool found = true;
    bool near = false;

    // Check if full tour
    for (auto &row : board) {
        for (auto cell : row) {
            if (cell == -1) {
                cout << "No solution found\n";
                found = false;
                break;
            }
        }
        if (!found) break;
    }

    // Check if closed tour
    for (auto [dx, dy] : knight_moves) {
        if (x + dx == start_x && y + dy == start_y) {
            near = true;
            break;
        }
    }

    if (found && near) {
        cout << "Knight's tour starting at (" << start_x << ", " << start_y << ")\n";
        cout << "Knight's tour end at (" << x << ", " << y << ")\n";
        cout << "Knight's tour found\n";
    } else {
        cout << "No knight's tour starting at (" << start_x << ", " << start_y << ")\n";
    }

    // Print board
    for (auto &row : board) {
        for (auto cell : row) {
            printf("%2d ", cell);
        }
        cout << "\n";
    }

    return 0;
}