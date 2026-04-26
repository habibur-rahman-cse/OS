#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct Process {
    int id, at, bt, pri;
    int rem;
    int ct, tat, wt;
    bool done = false;
};

// Non-Preemptive Priority
void nonPreemptive(vector<Process> &p, int n) {
    for (int i = 0; i < n; i++) { 
        p[i].done = false;
        p[i].rem = p[i].bt;
    }

    int time = 0, completed = 0;
    cout << "[Non-Preemptive] Gantt Chart: ";

    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time) {
                if (idx == -1 || p[i].pri < p[idx].pri)
                    idx = i;
            }
        }

        if (idx == -1) {
            int next = 1e9;
            for (int i = 0; i < n; i++)
                if (!p[i].done) next = min(next, p[i].at);
            time = next;
            continue;
        }

        cout << "| P" << p[idx].id << "(" << time << "-" << time + p[idx].bt << ") ";
        time       += p[idx].bt;
        p[idx].ct   = time;
        p[idx].tat  = p[idx].ct  - p[idx].at;
        p[idx].wt   = p[idx].tat - p[idx].bt;
        p[idx].done = true;
        completed++;
    }
    cout << "|\n\n";
}

// Preemptive Priority
void preemptive(vector<Process> &p, int n) {
    for (int i = 0; i < n; i++) {
        p[i].done = false;
        p[i].rem = p[i].bt;
    }

    int time = 0, completed = 0, prevIdx = -1;
    cout << "[Preemptive]     Gantt Chart: ";

    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].rem > 0 && p[i].at <= time) {
                if (idx == -1 || p[i].pri < p[idx].pri)
                    idx = i;
            }
        }

        if (idx == -1) { 
            time++; 
            continue;
        }

        if (idx != prevIdx) {
            if (prevIdx != -1) cout << time << ") ";
            cout << "| P" << p[idx].id << "(" << time << "-";
            prevIdx = idx;
        }

        p[idx].rem--;
        time++;

        if (p[idx].rem == 0) {
            cout << time << ") ";
            prevIdx    = -1;
            p[idx].ct  = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt  = p[idx].tat - p[idx].bt;
            p[idx].done = true;
            completed++;
        }
    }
    cout << "|\n\n";
}

// ─── Result Table ───────────────────────────────────────────
void printTable(vector<Process> &p, int n) {
    cout << "PID\tAT\tBT\tPRI\tCT\tTAT\tWT\n";

    float totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].id << "\t"
             << p[i].at  << "\t" << p[i].bt  << "\t" << p[i].pri << "\t"
             << p[i].ct  << "\t" << p[i].tat << "\t" << p[i].wt  << "\n";

        totalWT  += p[i].wt;
        totalTAT += p[i].tat;
    }

    cout << endl;
    cout << "Average Waiting Time    : " << totalWT  / n << "\n";
    cout << "Average Turnaround Time : " << totalTAT / n << "\n";
}

// ─── Main ───────────────────────────────────────────────────
int main() {
    int n;
    cin >> n;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cin >> p[i].at >> p[i].bt >> p[i].pri;
    }

    int choice;
    cin >> choice; // 1 for non-preemptive, 2 for preemptive

    if (choice == 1) { // non-preemptive
        nonPreemptive(p, n);
        printTable(p, n);
    }
    else if (choice == 2) { // preemptive
        preemptive(p, n);
        printTable(p, n);
    }

    return 0;
}

/*  ── SAMPLE RUN ──────────────────────────────────────────────
    Input : P1:AT=0,BT=6,PRI=3 | P2:AT=1,BT=4,PRI=1
            P3:AT=2,BT=2,PRI=4 | P4:AT=3,BT=5,PRI=2
            P5:AT=4,BT=1,PRI=5
    
5
0 6 3
1 4 1
2 2 4
3 5 2
4 1 5
2

    ── Non-Preemptive ──
    t=0: Only P1 arrived → run P1(0-6)
    t=6: All arrived → pick highest priority → P2(pri=1)
    t=10: P4(pri=2) → P4(10-15)
    t=15: P3(pri=4) → P3(15-17)  ...etc

    Gantt: |P1(0-6)|P2(6-10)|P4(10-15)|P3(15-17)|P5(17-18)|

    PID  AT  BT  PRI  CT  TAT  WT
    P1    0   6   3    6    6   0
    P2    1   4   1   10    9   5
    P3    2   2   4   17   15  13
    P4    3   5   2   15   12   7
    P5    4   1   5   18   14  13

    Avg WT  = 7.6
    Avg TAT = 11.2

    ── Preemptive ──
    t=1: P2(pri=1) arrives → preempts P1(pri=3)
    t=3: P4(pri=2) arrives → P2(pri=1) still runs
    t=5: P2 finishes → P4(pri=2) runs
    ... P1 resumes only after P4 finishes

    Avg WT  is lower due to preemption
    ────────────────────────────────────────────────────────── */
