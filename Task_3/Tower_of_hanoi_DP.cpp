#include <iostream>
#include <vector>
#include <cmath>

#define ll long long
#define endl '\n'

using namespace std;

ll cnt = 0;
vector <ll> dp, bestk;

void tower3_DP(int n, char from, char to, char aux, int offset)
{
    if (n == 0) return;
    tower3_DP(n - 1, from, aux, to, offset);
    cout << "Move disk " << n + offset << " from " << from << " to " << to << endl;
    cnt++;
    tower3_DP(n - 1, aux, to, from, offset);
}

void tower4_DP(int n, char from, char to, char aux1, char aux2, int offset)
{
    if (n == 0) return;
    if (n == 1)
    {
        cout << "Move disk " << offset + 1 << " from " << from << " to " << to << endl;
        cnt++;
        return;
    }
    
    int k = bestk[n];
    tower4_DP(n - k, from, aux1, aux2, to, offset);
    tower3_DP(k, from, to, aux2, offset + (n - k));
    tower4_DP(n - k, aux1, to, from, aux2, offset);
    return;
    
}
ll check_k(int k)
{
    if (k >= 63) return LLONG_MAX;
    return (1LL << k) - 1;
}

int main()
{
    int n;
    cin >> n;

    dp.assign(n + 1, LLONG_MAX);
    bestk.assign(n + 1, 0);

    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; i++)
    {
        for (int k = 1; k < i; k++)
        {
            if (check_k(k) == LLONG_MAX || dp[i-k] == LLONG_MAX)
            {
                continue;
            }
            ll moves = 2 * dp[i - k] + check_k(k);
            if (moves < dp[i])  
            {
                dp[i] = moves;
                bestk[i] = k;
            }
        }
    }


    tower4_DP(n, 'A', 'C', 'B', 'D', 0);
    cout << "Actual moves: " << cnt << endl;
    cout << "DP moves: " << dp[n] << endl;
    return 0;
}