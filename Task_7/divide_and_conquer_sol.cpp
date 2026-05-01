#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1e9; // Large number to represent infinity

struct Job {
    int id;
    int t;  // processing time
    int d;  // deadline
    int p;  // penalty
};

// The Divide and Conquer recursive function
int solveDC(vector<Job> remaining_jobs, int current_time) {
    // Base Case: No jobs left to schedule. The penalty is 0.
    if (remaining_jobs.empty()) {
        return 0;
    }

    int min_penalty = INF;

    // Divide: Try picking every available job as the *next* job in the sequence
    for (int i = 0; i < remaining_jobs.size(); ++i) {
        Job current_job = remaining_jobs[i];

        // 1. Calculate penalty for the chosen job right now
        int completion_time = current_time + current_job.t;
        int current_penalty = (completion_time > current_job.d) ? current_job.p : 0;

        // 2. Form the new subproblem (the list of remaining jobs WITHOUT the chosen one)
        vector<Job> next_remaining;
        for (int j = 0; j < remaining_jobs.size(); ++j) {
            if (i != j) { // Skip the job we just picked
                next_remaining.push_back(remaining_jobs[j]);
            }
        }

        // 3. Conquer: Solve the subproblem recursively
        int subproblem_penalty = solveDC(next_remaining, completion_time);

        // 4. Combine: Total penalty for this branch
        int total_penalty = current_penalty + subproblem_penalty;

        // Keep track of the minimum penalty across all choices
        min_penalty = min(min_penalty, total_penalty);
    }

    return min_penalty;
}

int main() {
    // The input from your test case
    vector<Job> jobs = {
        {1, 3, 4, 10},
        {2, 2, 2, 20},
        {3, 1, 3, 15}
    };

    // Start the recursion with all jobs available and current time = 0
    int optimal_penalty = solveDC(jobs, 0);

    cout << "Minimum Total Penalty (Divide & Conquer): " << optimal_penalty << endl;

    return 0;
}