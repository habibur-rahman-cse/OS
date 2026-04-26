#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct Process {
    int id, at, bt;
    int ct, tat, wt;
    bool done = false;
};

int main() {
    int n; // number of process
    cin >> n;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cin >> p[i].at >> p[i].bt;
    }

    int time = 0, completed = 0;
    cout << "Gantt Chart: ";

    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time) {
                if (idx == -1 || p[i].bt < p[idx].bt)
                    idx = i;
            }
        }

        if (idx == -1) {
            int nextAT = 1e9;
            for (int i = 0; i < n; i++)
                if (!p[i].done) nextAT = min(nextAT, p[i].at);

            cout << "| IDLE (" << time << "-" << nextAT << ") ";
            time = nextAT;
            continue;
        }

        // Run selected process to completion
        cout << "| P" << p[idx].id << "(" << time << "-" << time + p[idx].bt << ") ";
        time        += p[idx].bt;
        p[idx].ct    = time;
        p[idx].tat   = p[idx].ct - p[idx].at;
        p[idx].wt    = p[idx].tat - p[idx].bt;
        p[idx].done  = true;
        completed++;
    }
    cout << "|\n\n";

    // Output print
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

    At t=0  → only P1 arrived → run P1 (0-6)
    At t=6  → P2,P3,P4,P5 all arrived → pick shortest: P5(BT=1)
    At t=7  → P3(BT=2) next
    At t=9  → P2(BT=4) next
    At t=13 → P4(BT=5) last

5
0 6
1 4
2 2
3 5
4 1

    Gantt: | P1(0-6) | P5(6-7) | P3(7-9) | P2(9-13) | P4(13-18) |

    PID  AT  BT  CT  TAT  WT
    P1    0   6   6    6   0
    P2    1   4  13   12   8
    P3    2   2   9    7   5
    P4    3   5  18   15  10
    P5    4   1   7    3   2

    Avg WT  = 5.0
    Avg TAT = 8.6
    ────────────────────────────────────────────────────────── */
