#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m; // number of process & resources

    vector<int> avail(m);
    vector<vector<int>> alloc(n, vector<int> (m));
    vector<vector<int>> maxm = alloc, need = alloc;

    // alocation matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> alloc[i][j];

    // maximum matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> maxm[i][j];

    // avilable resourcse
    for (int j = 0; j < m; j++) {
        cin >> avail[j];
    }

    // Step 1 : Calculate Need matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maxm[i][j] - alloc[i][j];

    // Print Need matrix
    cout << "\nNeed matrix:\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i << " : ";
        for (int j = 0; j < m; j++) cout << need[i][j] << " ";
        cout << "\n";
    }

    vector<bool> finish(n, false);
    vector<int> work = avail;

    int count = 0;
    vector<int> safeSeq;
    cout << "\nSafety Algorithm Steps\n";

    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                // Check if Need[i] <= Work
                bool canRun = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canRun = false; 
                        break; 
                    }
                }
                if (canRun) {
                    // Simulate process finishing: add allocation back to work
                    cout << "P" << i << " can execute. Work before: [";
                    for (int j = 0; j < m; j++) cout << work[j] << (j<m-1?",":"");
                    cout << "]\n";

                    for (int j = 0; j < m; j++)
                        work[j] += alloc[i][j];

                    cout << "   P" << i << " finishes. Work after : [";
                    for (int j = 0; j < m; j++) cout << work[j] << (j<m-1?",":"");
                    cout << "]\n";

                    safeSeq.push_back(i);
                    finish[i] = true;
                    found = true;
                    count++;
                    break;
                }
            }
        }
        if (!found) break; // No process could run — unsafe
    }

    // Step 3 : Result
    if (count == n) {
        cout << "\nSystem is in SAFE STATE\n";
        cout << "Safe Sequence: ";
        for (int i = 0; i < n; i++)
            cout << "P" << safeSeq[i] << (i<n-1?" -> ":"");
        cout << "\n";
    } else {
        cout << "\nSystem is in UNSAFE STATE\n";
        cout << "Deadlock may occur!\n";
    }
    return 0;
}

/*  ── SAMPLE INPUT ────────────────────────────────────────────
    n=5, m=3
    Allocation:          Maximum:
    P0: 0 1 0            P0: 7 5 3
    P1: 2 0 0            P1: 3 2 2
    P2: 3 0 2            P2: 9 0 2
    P3: 2 1 1            P3: 2 2 2
    P4: 0 0 2            P4: 4 3 3
    Available: 3 3 2

5 3
0 1 0
2 0 0 
3 0 2
2 1 1
0 0 2

7 5 3
3 2 2
9 0 2
2 2 2
4 3 3

    Need matrix:
    P0: 7 4 3
    P1: 1 2 2
    P2: 6 0 0
    P3: 0 1 1
    P4: 4 3 1

    Safe Sequence: P1 -> P3 -> P4 -> P0 -> P2
    ────────────────────────────────────────────────────────── */
