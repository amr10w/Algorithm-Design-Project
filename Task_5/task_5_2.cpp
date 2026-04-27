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

void generateShootingSequence(int n,vector<int> &sequence)
{
    // Forward sweep: 2 to n-1
    for (int i = 2; i <= n - 1; i++)
        sequence.push_back(i);

    // Backward sweep: n-1 down to 2
    for (int i = n - 1; i >= 2; i--)
        sequence.push_back(i);
}

int main() {
    srand(time(0));

    cout << "Enter number of hiding spots (n > 1): ";
    cin >> n;

    targetPos = rand() % n + 1;
    cout << "Target initial position (hidden): " << targetPos << endl << endl;

    // Generate sequence: 2, 3, 4, ..., n-1, n-1, n-2, ..., 3, 2
    vector<int> sequence;

    generateShootingSequence(n,sequence);

    // Print sequence
    cout << "Shot sequence: ";
    for (int i = 0; i < (int)sequence.size(); i++)
        cout << sequence[i] << " ";
    cout << endl;
    cout << "Sequence length: " << sequence.size() << endl << endl;

    // Simulate
    bool hit = false;
    for (int i = 0; i < (int)sequence.size(); i++) {
        cout << "Shot #" << i + 1 << " -> Shoot at " << sequence[i] << " | Target at " << targetPos;

        if (sequence[i] == targetPos) {
            cout << " | *** HIT! ***" << endl;
            hit = true;
            break;
        }
        cout << " | Miss" << endl;
        moveTarget();
    }

    cout << endl;
    if (hit)
        cout << "HIT!" << endl;
    else
        cout << "NOT HIT" << endl;

    return 0;
}