#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct Process {
    int id, at, bt;
    int rem;          // remaining burst time
    int ct, tat, wt;
    bool started = false;
};

int main() {
    int n; // number of process
    cin >> n;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id  = i + 1;
        cin >> p[i].at >> p[i].bt; // arival time, burst time
        p[i].rem = p[i].bt; // remaining time
    }

    int totalBurst = 0;
    for (int i = 0; i < n; i++) {
        totalBurst += p[i].bt;
    }

    int time = 0, completed = 0;
    int prevIdx = -1;
    bool isIdle = false;
    cout << "Gantt Chart: ";

    while (completed < n) {
        // Pick process with smallest remaining time (arrived, not done)
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].rem > 0 && p[i].at <= time) {
                if (idx == -1 || p[i].rem < p[idx].rem)
                    idx = i;
            }
        }

        if (idx == -1) {
            if (!isIdle) {
                if (prevIdx != -1) cout << time << ") ";
                cout << "| Idle(" << time << "-";
                isIdle = true;
                prevIdx = -1;
            }
            time++;
            continue;
        }
        if (isIdle) {
            cout << time << ") ";
            isIdle = false;
        }

        if (idx != prevIdx) { // print Gantt segment only when process changes
            if (prevIdx != -1) cout << time << ") ";
            cout << "| P" << p[idx].id << "(" << time << "-";
            prevIdx = idx;
        }

        p[idx].rem--;
        time++;

        if (p[idx].rem == 0) { // Project finished?
            cout << time << ") ";
            prevIdx    = -1;
            p[idx].ct  = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt  = p[idx].tat - p[idx].bt;
            completed++;
        }
    }
    cout << "|\n\n";

    // Print results
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";
    float totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].id << "\t"
             << p[i].at  << "\t" << p[i].bt  << "\t"
             << p[i].ct  << "\t" << p[i].tat << "\t" << p[i].wt << "\n";
        totalWT  += p[i].wt;
        totalTAT += p[i].tat;
    }
    cout << endl;
    cout << "Average Waiting Time    : " << totalWT  / n << "\n";
    cout << "Average Turnaround Time : " << totalTAT / n << "\n";
    return 0;
}

/*  ── SAMPLE RUN ──────────────────────────────────────────────
    Input : P1:AT=0,BT=6 | P2:AT=1,BT=4 | P3:AT=2,BT=2
            P4:AT=3,BT=5 | P5:AT=4,BT=1

    t=0: P1(rem=6) only → run P1
    t=1: P2(rem=4) < P1(rem=5) → preempt → run P2
    t=2: P3(rem=2) < P2(rem=3) → preempt → run P3
    t=3: P3(rem=1) < all others → continue P3
    t=4: P5(rem=1) arrives, ties P3 → P3 finishes first
    ... and so on

5
0 6
1 4
2 2
3 5
4 1

    Gantt: |P1(0-1)|P2(1-2)|P3(2-4)|P5(4-5)|P2(5-8)|P4(8-13)|P1(13-18)|

    PID  AT  BT  CT  TAT  WT
    P1    0   6  18   18  12
    P2    1   4   8    7   3
    P3    2   2   4    2   0
    P4    3   5  13   10   5
    P5    4   1   5    1   0

    Avg WT  = 4.0
    Avg TAT = 7.6
    ────────────────────────────────────────────────────────── */
