#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int targetPos;
int n;

void moveTarget() {
    if (targetPos == 1)
        targetPos = 2;
    else if (targetPos == n)
        targetPos = n - 1;
    else {
        if (rand() % 2 == 0)
            targetPos--;
        else
            targetPos++;
    }
}

bool search(vector<int>& sequence, int lo, int hi) {
    if (lo > hi)
        return false;

    int mid = lo + (hi - lo) / 2;

    sequence.push_back(mid);
    cout << "Shot #" << sequence.size() << " -> Shoot at " << mid << " | Target at " << targetPos;

    if (mid == targetPos) {
        cout << " | *** HIT! ***" << endl;
        return true;
    }
    cout << " | Miss" << endl;
    moveTarget();

    // Guess: search left half
    if (search(sequence, lo, mid - 1))
        return true;

    // If left failed, try right
    if (search(sequence, mid + 1, hi))
        return true;

    return false;
}

int main() {
    srand(time(0));

    cout << "Enter number of hiding spots (n > 1): ";
    cin >> n;

    targetPos = rand() % n + 1;
    cout << "Target initial position (hidden): " << targetPos << endl << endl;

    vector<int> sequence;
    bool hit = search(sequence, 1, n);

    cout << endl;
    cout << "Shot sequence: ";
    for (int i = 0; i < (int)sequence.size(); i++)
        cout << sequence[i] << " ";
    cout << endl;

    if (hit)
        cout << "HIT after " << sequence.size() << " shots!" << endl;
    else
        cout << "NOT HIT" << endl;

    return 0;
}