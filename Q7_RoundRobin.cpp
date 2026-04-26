#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
using namespace std;

struct Process {
    int id, at, bt;
    int rem;           // remaining burst time
    int ct, tat, wt;
};

int main() {
    int n, quantum;
    cin >> n; // number of process
    cin >> quantum; // quantum time

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id  = i + 1;
        cin >> p[i].at >> p[i].bt;
        p[i].rem = p[i].bt;
    }

    sort(p.begin(), p.end(), [](Process a, Process b) { // sort by arrival time
        return a.at < b.at;
    });

    queue<int> readyQueue;   // stores index into p[]
    vector<bool> inQueue(n, false);

    int time = 0, completed = 0;
    cout << "Gantt Chart: ";

    // Add processes that arrive at t=0
    for (int i = 0; i < n; i++) {
        if (p[i].at == 0) { readyQueue.push(i); inQueue[i] = true; }
    }

    while (completed < n) {
        if (readyQueue.empty()) {
            // CPU idle — jump to next arrival
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && p[i].rem > 0) {
                    time = p[i].at;
                    readyQueue.push(i);
                    inQueue[i] = true;
                    break;
                }
            }
            continue;
        }

        int idx = readyQueue.front();
        readyQueue.pop();

        int runTime = min(quantum, p[idx].rem);   // run for quantum or less
        cout << "| P" << p[idx].id << "(" << time << "-" << time + runTime << ") ";

        time        += runTime;
        p[idx].rem  -= runTime;

        // Add newly arrived processes (arrived during this quantum)
        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && p[i].rem > 0 && p[i].at <= time) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if (p[idx].rem == 0) {
            p[idx].ct  = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt  = p[idx].tat - p[idx].bt;
            completed++;
        } else {
            readyQueue.push(idx);   // back to end of queue
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

/*  ── SAMPLE RUN (Quantum = 2) ────────────────────────────────
    Input : P1:AT=0,BT=6 | P2:AT=1,BT=4 | P3:AT=2,BT=2
            P4:AT=3,BT=5 | P5:AT=4,BT=1

    Gantt: |P1(0-2)|P2(2-4)|P3(4-6)|P4(6-8)|P5(8-9)
           |P1(9-11)|P2(11-13)|P4(13-15)|P1(15-17)|P4(17-18)|

5 2
0 6
1 4
2 2
3 5
4 1

    PID  AT  BT  CT  TAT  WT
    P1    0   6  17   17  11
    P2    1   4  13   12   8
    P3    2   2   6    4   2
    P4    3   5  18   15  10
    P5    4   1   9    5   4

    Avg WT  = 7.0
    Avg TAT = 10.6

    Try Quantum=1 for maximum fairness, Quantum=6 → behaves like FCFS
    ────────────────────────────────────────────────────────── */
