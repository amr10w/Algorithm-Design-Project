#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <float.h>

using namespace std;
using namespace std::chrono;

struct Point {
    double x, y;
};

double getDist(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

double calculateSSD(const vector<Point>& points, const vector<int>& groups, int k) {
    double totalSSD = 0;
    for (int i = 0; i < k; i++) {
        double sumX = 0, sumY = 0;
        int count = 0;
        for (int j = 0; j < (int)points.size(); j++) {
            if (groups[j] == i) {
                sumX += points[j].x;
                sumY += points[j].y;
                count++;
            }
        }
        if (count > 0) {
            Point center = {sumX / count, sumY / count};
            for (int j = 0; j < (int)points.size(); j++) {
                if (groups[j] == i) totalSSD += getDist(points[j], center);
            }
        }
    }
    return totalSSD;
}

// 1. Brute Force
double bestBF_SSD = DBL_MAX;
void runBruteForce(const vector<Point>& pts, vector<int>& assign, int idx, int k) {
    if (idx == (int)pts.size()) {
        double currentSSD = calculateSSD(pts, assign, k);
        if (currentSSD < bestBF_SSD) bestBF_SSD = currentSSD;
        return;
    }
    for (int i = 0; i < k; i++) {
        assign[idx] = i;
        runBruteForce(pts, assign, idx + 1, k);
    }
}

// 2. Iterative Improvement
double runIterativeImprovement(const vector<Point>& pts, int k) {
    int n = pts.size();
    vector<Point> centers(k);
    vector<int> assign(n);
    for (int i = 0; i < k; i++) centers[i] = pts[i];

    for (int iter = 0; iter < 50; iter++) {
        for (int i = 0; i < n; i++) {
            double dMin = DBL_MAX;
            for (int j = 0; j < k; j++) {
                double d = getDist(pts[i], centers[j]);
                if (d < dMin) { dMin = d; assign[i] = j; }
            }
        }
        vector<double> sx(k, 0), sy(k, 0), count(k, 0);
        for (int i = 0; i < n; i++) {
            sx[assign[i]] += pts[i].x;
            sy[assign[i]] += pts[i].y;
            count[assign[i]]++;
        }
        for (int i = 0; i < k; i++) {
            if (count[i] > 0) centers[i] = {sx[i] / count[i], sy[i] / count[i]};
        }
    }
    return calculateSSD(pts, assign, k);
}

// 3. Divide & Conquer
vector<Point> runDNC(vector<Point> pts, int k) {
    if ((int)pts.size() <= k) return pts;
    sort(pts.begin(), pts.end(), [](Point a, Point b) { return a.x < b.x; });
    int mid = pts.size() / 2;
    vector<Point> leftSide(pts.begin(), pts.begin() + mid);
    vector<Point> rightSide(pts.begin() + mid, pts.end());
    vector<Point> leftRes = runDNC(leftSide, k);
    vector<Point> rightRes = runDNC(rightSide, k);
    leftRes.insert(leftRes.end(), rightRes.begin(), rightRes.end());
    if ((int)leftRes.size() > k) leftRes.resize(k); 
    return leftRes; 
}

void runTest(string title, vector<Point> data, int k) {
    cout << "\n--------------------------------------------------" << endl;
    cout << ">>> " << title << endl;

    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();

    bestBF_SSD = DBL_MAX;
    vector<int> bfAssign(data.size());
    start = high_resolution_clock::now();
    runBruteForce(data, bfAssign, 0, k);
    stop = high_resolution_clock::now();
    cout << "Brute Force Result       -> SSD: " << bestBF_SSD << " | Time: " << duration_cast<microseconds>(stop - start).count() << " us" << endl;

    start = high_resolution_clock::now();
    double iterativeSSD = runIterativeImprovement(data, k);
    stop = high_resolution_clock::now();
    cout << "Iterative Improvement    -> SSD: " << iterativeSSD << " | Time: " << duration_cast<microseconds>(stop - start).count() << " us" << endl;

    start = high_resolution_clock::now();
    vector<Point> dncCenters = runDNC(data, k);
    vector<int> dncAssign(data.size());
    for(int i=0; i<(int)data.size(); i++) {
        double dMin = DBL_MAX;
        for(int j=0; j<(int)dncCenters.size(); j++) {
            double d = getDist(data[i], dncCenters[j]);
            if(d < dMin) { dMin = d; dncAssign[i] = j; }
        }
    }
    double dncSSD = calculateSSD(data, dncAssign, k);
    stop = high_resolution_clock::now();
    cout << "Divide & Conquer         -> SSD: " << dncSSD << " | Time: " << duration_cast<microseconds>(stop - start).count() << " us" << endl;
}

int main() {
    runTest("Test Case 1: Small Group", {{1,1}, {1.2,1}, {5,5}, {5,5.5}}, 2);
    runTest("Test Case 2: Very Close Points", {{1,1}, {1.1,1.1}, {1.2,1.2}, {1.3,1.3}}, 2);
    runTest("Test Case 3: 8-Point Dataset", {{1,1}, {1,2}, {2,1}, {5,5}, {6,6}, {5,6}, {10,10}, {11,11}}, 3);
    return 0;
}