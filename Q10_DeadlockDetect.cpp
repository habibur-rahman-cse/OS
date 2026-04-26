#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m; // processes & resources

    vector<int> avail(m);
    vector<vector<int>> alloc(n, vector<int>(m));
    vector<vector<int>> request = alloc;

    // allocation matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> alloc[i][j];

    // request matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> request[i][j];

    // avail matrix
    for (int j = 0; j < m; j++)
        cin >> avail[j];

    vector<bool> finish(n);

    for (int i = 0; i < n; i++) {
        finish[i] = true;
        for(int j = 0 ; j < m ; j++)
            if(request[i][j] > 0)
                finish[i] = false;

        if (finish[i]) {
            cout << "P" << i << " has no pending request — marked finished\n";
        }
    }

    vector<int> work = avail;

    // Step 2 : Detection loop
    cout << "\n--- Detection Steps ---\n";
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canRun = true;
                for (int j = 0; j < m; j++) {
                    if (request[i][j] > work[j]) { 
                        canRun = false; 
                        break; 
                    }
                }
                if (canRun) {
                    cout << "P" << i << " request [";
                    for (int j = 0; j < m; j++) cout << request[i][j] << (j<m-1?",":"");
                    cout << "] can be granted. \n";

                    for (int j = 0; j < m; j++)
                        work[j] += alloc[i][j];

                    cout << "   P" << i << " finishes. Work after : [";
                    for (int j = 0; j < m; j++) cout << work[j] << (j<m-1?",":"");
                    cout << "]\n";

                    finish[i] = true;
                    changed = true;
                }
            }
        }
    }

    cout << endl;
    // Step 3 : Find deadlocked processes
    bool deadlock = false;
    for (int i = 0; i < n; i++) {
        if (!finish[i]) {
            if (!deadlock) cout << "DEADLOCK DETECTED!\nDeadlocked processes: ";
            cout << "P" << i << " ";
            deadlock = true;
        }
    }
    if (deadlock) cout << "\n";
    else cout << "No deadlock detected. All processes can complete.\n";
    
    return 0;
}

/*  ── SAMPLE INPUT ────────────────────────────────────────────
    n=5, m=3
    Allocation:          Request:
    P0: 0 1 0            P0: 0 0 0   (no request → already done)
    P1: 2 0 0            P1: 2 0 2
    P2: 3 0 3            P2: 0 0 0   (no request → already done)
    P3: 2 1 1            P3: 1 0 0
    P4: 0 0 2            P4: 0 0 2
    Available: 0 0 0

    P0 and P2 have zero requests → marked finished
    P1: Request [2,0,2] > Available [0,0,0] → blocked
    P3: Request [1,0,0] > Available [0,0,0] → blocked
    P4: Request [0,0,2] > Available [0,0,0] → blocked

    DEADLOCK DETECTED! Deadlocked processes: P1 P3 P4
    ────────────────────────────────────────────────────────── */
