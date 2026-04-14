#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

const int INF = 1e9;

struct Point {
    int x, y;
};

// Returns (minimum_lines, path_of_vertices)
pair<int, vector<Point>> find_minimum_segments(int n) {
    // All the dots we actually need to cover (the "targets")
    vector<Point> targets;
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            targets.push_back({x, y});
        }
    }
    
    int num_targets = targets.size();
    int all_targets_mask = (1 << num_targets) - 1;

    // Add a 2-unit padding around the grid so we can turn outside
    int padding = 2;
    vector<Point> vertices;
    for (int x = -padding; x < n + padding; ++x) {
        for (int y = -padding; y < n + padding; ++y) {
            vertices.push_back({x, y});
        }
    }
    int V = vertices.size();

    // For every pair of vertices, which targets lie exactly on the line between them?
    vector<vector<int>> targets_on_edge(V, vector<int>(V, 0));
    
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (i == j) continue;
            
            Point& from = vertices[i];
            Point& to = vertices[j];
            int mask = 0;
            
            for (int t = 0; t < num_targets; ++t) {
                Point& target = targets[t];
                
                // Cross product = 0 means collinear
                long long cross = 1LL * (target.y - from.y) * (to.x - from.x) 
                                - 1LL * (target.x - from.x) * (to.y - from.y);
                
                if (cross == 0) {
                    // Also check it's between the two vertices (not outside)
                    if (target.x >= min(from.x, to.x) && target.x <= max(from.x, to.x) &&
                        target.y >= min(from.y, to.y) && target.y <= max(from.y, to.y)) {
                        mask |= (1 << t);
                    }
                }
            }
            targets_on_edge[i][j] = mask;
        }
    }

    // DP table: dp[vertex][mask] = minimum segments to reach this vertex covering 'mask' targets
    vector<vector<int>> dp(V, vector<int>(all_targets_mask + 1, INF));
    
    // For backtracking: parent[vertex][mask] = (previous_vertex, previous_mask)
    vector<vector<pair<int, int>>> parent(V, vector<pair<int, int>>(all_targets_mask + 1, {-1, -1}));

    // Base case: any single segment that covers at least one target
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (i != j && targets_on_edge[i][j] > 0) {
                dp[j][targets_on_edge[i][j]] = 1;
                parent[j][targets_on_edge[i][j]] = {i, 0};
            }
        }
    }

    // Main DP: try extending paths
    for (int mask = 1; mask < all_targets_mask; ++mask) {
        for (int v = 0; v < V; ++v) {
            if (dp[v][mask] == INF) continue;
            
            for (int u = 0; u < V; ++u) {
                if (v == u) continue;
                
                int new_mask = mask | targets_on_edge[v][u];
                if (new_mask == mask) continue;  // No new targets? Skip.
                
                if (dp[v][mask] + 1 < dp[u][new_mask]) {
                    dp[u][new_mask] = dp[v][mask] + 1;
                    parent[u][new_mask] = {v, mask};
                }
            }
        }
    }

    // Find the best end vertex (smallest number of segments covering all targets)
    int best_segments = INF;
    int best_vertex = -1;
    for (int v = 0; v < V; ++v) {
        if (dp[v][all_targets_mask] < best_segments) {
            best_segments = dp[v][all_targets_mask];
            best_vertex = v;
        }
    }

    if (best_vertex == -1) {
        return {INF, {}};  // No solution found
    }

    // Reconstruct the path
    vector<Point> path;
    int curr_v = best_vertex;
    int curr_mask = all_targets_mask;
    
    while (curr_mask > 0) {
        path.push_back(vertices[curr_v]);
        auto [prev_v, prev_mask] = parent[curr_v][curr_mask];
        curr_v = prev_v;
        curr_mask = prev_mask;
    }
    path.push_back(vertices[curr_v]);  // Add the starting vertex
    reverse(path.begin(), path.end());

    return {best_segments, path};
}

int main() {
    int n = 4;
    auto [lines, path] = find_minimum_segments(n);
    
    cout << "Minimum lines needed: " << lines << endl;
    cout << "Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << "(" << path[i].x << ", " << path[i].y << ")";
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << endl;
    
    return 0;
}