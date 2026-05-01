#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Job {
    int id; // 1-based index 
    int t;  // processing time
    int d;  // deadline
    int p;  // penalty
};

int main() {
    // Your specific input
    vector<Job> jobs = {
        {1, 3, 6, 10},  
        {2, 2, 2, 20},  
        {3, 1, 5, 30},  
        {4, 2, 7, 15}   
    };

    int n = jobs.size();
    int min_total_penalty = 1e9; 
    vector<int> best_sequence;   

    // Create an array of indices [0, 1, 2] to represent the job permutations
    vector<int> p(n);
    for (int i = 0; i < n; ++i) {
        p[i] = i; 
    }



    do {
        int current_time = 0;
        int current_penalty = 0;

        // Calculate the penalty for the current permutation
        for (int i = 0; i < n; ++i) {
            int job_idx = p[i]; // Get the actual job index from the permutation array
            
            current_time += jobs[job_idx].t; // Advance time by job's duration
            
            // If the job finishes after its deadline, add its penalty
            if (current_time > jobs[job_idx].d) {
                current_penalty += jobs[job_idx].p;
            }
        }

        // If this sequence yields a lower penalty, save it
        if (current_penalty < min_total_penalty) {
            min_total_penalty = current_penalty;
            best_sequence = p;
        }

    } while (next_permutation(p.begin(), p.end()));

   
    cout << "Minimum Total Penalty: " << min_total_penalty << endl;
    cout << "Optimal Job Sequence: ";
    for (int idx : best_sequence) {
        cout << jobs[idx].id << " ";
    }
    cout << endl;

    return 0;
}