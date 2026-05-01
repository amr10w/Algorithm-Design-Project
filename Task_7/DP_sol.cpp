#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// A large number to represent infinity for our minimization logic
const int INF = 1e9; 

struct Job {
    int id; // To keep track of the original job number
    int t;  // processing time
    int d;  // deadline
    int p;  // penalty
};

// Helper function to calculate total time elapsed for the completed jobs in the mask
int calculateCurrentTime(int mask, const vector<Job>& jobs) {
    int currentTime = 0;
    for (int i = 0; i < jobs.size(); ++i) {
        // If the i-th bit is 1, the job is completed, so add its time
        if (mask & (1 << i)) { 
            currentTime += jobs[i].t;
        }
    }
    return currentTime;
}

// The core Top-Down DP function
int solveDP(int mask, const vector<Job>& jobs, vector<int>& memo) {
    int n = jobs.size();

    // Base Case: If the mask has all bits set to 1, all jobs are done.
    if (mask == (1 << n) - 1) {
        return 0;
    }

    // Memoization Check: If we've seen this exact state before, return it.
    if (memo[mask] != -1) {
        return memo[mask];
    }

    int currentTime = calculateCurrentTime(mask, jobs);
    int minPenalty = INF;

    // Explore choices: Try scheduling every available job as the *next* job
    for (int i = 0; i < n; ++i) {
        // Check if the i-th job is NOT done (the bit is 0)
        if (!(mask & (1 << i))) {
            
            // 1. Calculate the penalty for picking this job right now
            int completionTime = currentTime + jobs[i].t;
            int currentPenalty = (completionTime > jobs[i].d) ? jobs[i].p : 0;

            // 2. Turn on the i-th bit to mark it as done for the next recursive step
            int nextMask = mask | (1 << i);

            // 3. Recursively calculate the penalty of the remaining jobs
            int totalPenalty = currentPenalty + solveDP(nextMask, jobs, memo);

            // 4. Keep the minimum penalty found across all choices
            minPenalty = min(minPenalty, totalPenalty);
        }
    }

    // Save to memo table and return
    return memo[mask] = minPenalty;
}

int main() {
  
    // Job(id, time, deadline, penalty)
    vector<Job> jobs = {
        {1, 3, 4, 10},
        {2, 2, 3, 7},
        {3, 1, 6, 3}
    };

    int n = jobs.size();

    // Initialize memoization table with -1. 
    // Size is 2^n, which is written as (1 << n) in bitwise operations.
    vector<int> memo(1 << n, -1);

    // Start with mask 0 (no jobs completed)
    int initialMask = 0;
    int optimalPenalty = solveDP(initialMask, jobs, memo);

    cout << "Total Jobs: " << n << endl;
    cout << "Minimum Total Penalty: " << optimalPenalty << endl;

    return 0;
}