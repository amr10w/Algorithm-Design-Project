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

    if (lo == hi) {
        sequence.push_back(lo);
        cout << "Shot #" << sequence.size() << " -> Shoot at " << lo << " | Target at " << targetPos;
        if (lo == targetPos) {
            cout << " | *** HIT! ***" << endl;
            return true;
        }
        cout << " | Miss" << endl;
        moveTarget();
        return false;
    }

    int mid = lo + (hi - lo) / 2;
    int size = hi - lo + 1;

    // DIVIDE at mid
    // CONQUER LEFT [lo, mid]: sweep it 'size' times to guarantee catch
    int leftSize = mid - lo + 1;
    for (int pass = 0; pass < size; pass++) {
        if (pass % 2 == 0) {
            for (int i = lo; i <= mid; i++) {
                sequence.push_back(i);
                cout << "Shot #" << sequence.size() << " -> Shoot at " << i << " | Target at " << targetPos;
                if (i == targetPos) {
                    cout << " | *** HIT! ***" << endl;
                    return true;
                }
                cout << " | Miss" << endl;
                moveTarget();
            }
        } else {
            for (int i = mid; i >= lo; i--) {
                sequence.push_back(i);
                cout << "Shot #" << sequence.size() << " -> Shoot at " << i << " | Target at " << targetPos;
                if (i == targetPos) {
                    cout << " | *** HIT! ***" << endl;
                    return true;
                }
                cout << " | Miss" << endl;
                moveTarget();
            }
        }
    }

    // CONQUER RIGHT [mid+1, hi]: sweep it 'size' times to guarantee catch
    int rightSize = hi - mid;
    for (int pass = 0; pass < size; pass++) {
        if (pass % 2 == 0) {
            for (int i = mid + 1; i <= hi; i++) {
                sequence.push_back(i);
                cout << "Shot #" << sequence.size() << " -> Shoot at " << i << " | Target at " << targetPos;
                if (i == targetPos) {
                    cout << " | *** HIT! ***" << endl;
                    return true;
                }
                cout << " | Miss" << endl;
                moveTarget();
            }
        } else {
            for (int i = hi; i >= mid + 1; i--) {
                sequence.push_back(i);
                cout << "Shot #" << sequence.size() << " -> Shoot at " << i << " | Target at " << targetPos;
                if (i == targetPos) {
                    cout << " | *** HIT! ***" << endl;
                    return true;
                }
                cout << " | Miss" << endl;
                moveTarget();
            }
        }
    }

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