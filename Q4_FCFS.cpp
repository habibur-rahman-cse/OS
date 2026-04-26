#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;

struct Process {
    int id, at, bt;   // arrival time, burst time
    int ct, tat, wt;  // completion, turnaround, waiting
};
// First Come First Service

int main() {
    int n = 5;
    cin >> n;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cin >> p[i].at >> p[i].bt; // taking input of arival time & burst time
    }

    sort(p.begin(), p.end(), [] (Process a, Process b) {
        return a.at < b.at;
    });

    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].at) time = p[i].at;  // CPU idle gap
        time    += p[i].bt;
        p[i].ct  = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt  = p[i].tat - p[i].bt;
    }

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
    cout << fixed << setprecision(3);
    cout << "Average Waiting Time    : " << totalWT  / n << "\n";
    cout << "Average Turnaround Time : " << totalTAT / n << "\n";
    return 0;
}

/*  ── SAMPLE RUN ──────────────────────────────────────────────
    Input : 5 processes
    P1: AT=0 BT=6 | P2: AT=1 BT=4 | P3: AT=2 BT=2
    P4: AT=3 BT=5 | P5: AT=4 BT=1

5
0 6
1 4
2 2
3 5
4 1

    Gantt: | P1(0-6) | P2(6-10) | P3(10-12) | P4(12-17) | P5(17-18) |

    PID  AT  BT  CT  TAT  WT
    P1    0   6   6    6   0
    P2    1   4  10    9   5
    P3    2   2  12   10   8
    P4    3   5  17   14   9
    P5    4   1  18   14  13

    Avg WT  = 7.0
    Avg TAT = 10.6
    ────────────────────────────────────────────────────────── */


