#include <iostream>
#include <cmath>

#define ll long long
#define endl '\n'

using namespace std;

int cnt = 0;

void tower3_Divideandconquer(int n, char from, char to, char aux, int offset)
{
    if (n == 0) return;
    tower3_Divideandconquer(n - 1, from, aux, to, offset);
    cout << "Move disk " << n + offset << " from " << from << " to " << to << endl;
    cnt++;
    tower3_Divideandconquer(n - 1, aux, to, from, offset);
}

void tower4_Divideandconquer(int n, char from, char to, char aux1, char aux2, int offset)
{
    if (n == 0) return;
    if (n == 1)
    {
        cout << "Move disk " << offset + 1 << " from " << from << " to " << to << endl;
        cnt++;
        return;
    }
    if (n == 2)
    {
        cout << "Move disk " << offset + 1 << " from " << from << " to " << aux1 << endl;
        cout << "Move disk " << offset + 2 << " from " << from << " to " << to << endl;
        cout << "Move disk " << offset + 1 << " from " << aux1 << " to " << to << endl;
        cnt += 3;
        return;
    }
    int k = sqrt(2 * n);
    tower4_Divideandconquer(n - k, from, aux1, aux2, to, offset);
    tower3_Divideandconquer(k, from, to, aux2, offset + (n - k));
    tower4_Divideandconquer(n - k, aux1, to, from, aux2, offset);
    return;
    
}

int main()
{
    int n = 80;
    tower4_Divideandconquer(n, 'A', 'C', 'B', 'D', 0);
    cout << "Total moves: " << cnt << endl;
    return 0;
}