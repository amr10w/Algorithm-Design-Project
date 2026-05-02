#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <chrono> 

using namespace std;
using namespace std::chrono;

struct Edge {
    int to;
    int weight;
};

void printPartitions(int n, const vector<int>& partition) {
    cout << "Set A: { ";
    for (int i = 1; i <= n; i++) if (partition[i] == 0) cout << i << " ";
    cout << "}\nSet B: { ";
    for (int i = 1; i <= n; i++) if (partition[i] == 1) cout << i << " ";
    cout << "}" << endl;
}

long long calculateWeight(int n, const vector<vector<Edge>>& adj, const vector<int>& partition) {
    long long weight = 0;
    for (int u = 1; u <= n; u++) {
        for (auto& edge : adj[u]) {
            if (u < edge.to && partition[u] != partition[edge.to]) {
                weight += edge.weight;
            }
        }
    }
    return weight;
}

void solveBruteForce(int n, const vector<vector<Edge>>& adj) {
    auto start = high_resolution_clock::now();
    
    long long minCut = LLONG_MAX;
    vector<int> bestPartition(n + 1);

    for (int i = 1; i < (1 << (n - 1)); i++) {
        vector<int> currentPartition(n + 1);
        for (int j = 1; j <= n; j++) {
            currentPartition[j] = (i >> (j - 1)) & 1;
        }
        
        long long currentWeight = calculateWeight(n, adj, currentPartition);
        if (currentWeight < minCut) {
            minCut = currentWeight;
            bestPartition = currentPartition;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "--- Brute Force Result ---" << endl;
    cout << "Min Cut Weight: " << minCut << endl;
    cout << "Time taken: " << duration.count() << " microseconds" << endl;
    printPartitions(n, bestPartition);
}

void solveIterative(int n, const vector<vector<Edge>>& adj) {
    auto start = high_resolution_clock::now();

    vector<int> partition(n + 1, 0);
    for (int i = 1; i <= n / 2; i++) partition[i] = 1;

    bool improved = true;
    while (improved) {
        improved = false;
        for (int u = 1; u <= n; u++) {
            long long internal = 0, external = 0;
            for (auto& edge : adj[u]) {
                if (partition[u] == partition[edge.to]) internal += edge.weight;
                else external += edge.weight;
            }

            if (external > internal) {
                partition[u] = 1 - partition[u];
                improved = true;
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "\n--- Iterative Improvement Result ---" << endl;
    cout << "Approximate Weight: " << calculateWeight(n, adj, partition) << endl;
    cout << "Time taken: " << duration.count() << " microseconds" << endl;
    printPartitions(n, partition);
}

int main() {
    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    solveBruteForce(n, adj);
    solveIterative(n, adj);

    return 0;
}

