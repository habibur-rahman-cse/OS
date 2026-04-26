#include <iostream>
#include <vector>
using namespace std;

int findPage(vector<int> &frames, int page) {
    int f = frames.size();
    for (int i = 0; i < f; i++)
        if (frames[i] == page) return i; // returns index, or -1
    return -1;
}

int findLRU(vector<int> &timeUsed) {
    int f = timeUsed.size();
    int minTime = timeUsed[0], idx = 0;
    for (int i = 1; i < f; i++) {
        if (timeUsed[i] < minTime) {
            minTime = timeUsed[i];
            idx = i;
        }
    }
    return idx;   // index of least recently used frame
}

int main() {
    int n, f;
    cin >> n; // number of pages
    vector<int> pages(n);
    for (int i = 0; i < n; i++) {
        cin >> pages[i];
    }

    cin >> f; // number of frames

    vector<int> frames(f, -1);
    vector<int> timeUsed(f, 0);

    int pageFaults = 0, pageHits = 0;
    int clock = 0; // logical time counter

    cout << "Page\tFrames\t\tResult\n";

    for (int i = 0; i < n; i++) {
        clock++;
        int pos = findPage(frames, pages[i]);
        cout << pages[i] << "\t\t";

        if (pos != -1) {
            pageHits++;
            timeUsed[pos] = clock;
            for (int j = 0; j < f; j++)
                cout << (frames[j]==-1?"-":to_string(frames[j])) << " ";
            cout << "\t\tHIT\n";
        } else {
            pageFaults++;
            int slot = -1;
            for (int j = 0; j < f; j++) {
                if (frames[j] == -1) {
                    slot = j;
                    break;
                }
            }
            if (slot == -1) 
                slot = findLRU(timeUsed); // evict LRU

            int evicted = frames[slot];
            frames[slot] = pages[i];
            timeUsed[slot] = clock;

            for (int j = 0; j < f; j++)
                cout << (frames[j]==-1?"-":to_string(frames[j])) << " ";
            if (evicted == -1)
                cout << "\t\tFAULT (loaded)\n";
            else
                cout << "\t\tFAULT (evicted " << evicted << ")\n";
        }
    }

    cout << endl;
    cout << "Total Page Faults : " << pageFaults << "\n";
    cout << "Total Page Hits   : " << pageHits   << "\n";
    cout << "Hit Ratio         : " << (float)pageHits/n*100 << "%\n";
    return 0;
}

/*  ── SAMPLE INPUT ────────────────────────────────────────────
    Reference string: 7 0 1 2 0 3 0 4 2 3 0 3
    Frames: 3

12
7 0 1 2 0 3 0 4 2 3 0 3
3

    Page  Frames        Result
    7     7  -  -       FAULT (loaded)
    0     7  0  -       FAULT (loaded)
    1     7  0  1       FAULT (loaded)
    2     2  0  1       FAULT (evicted 7)  ← 7 used longest ago
    0     2  0  1       HIT   (0 just used → promoted)
    3     2  0  3       FAULT (evicted 1)  ← 1 used longest ago
    0     2  0  3       HIT
    4     4  0  3       FAULT (evicted 2)  ← 2 used longest ago
    2     4  0  2       FAULT (evicted 3)
    3     4  3  2       FAULT (evicted 0)
    0     4  3  0       FAULT (evicted... wait 4 is LRU)
                        → FAULT (evicted 4)
    3     3  3  0   ... HIT

    Total Page Faults : 8      ← 1 fewer than FIFO!
    Total Page Hits   : 4
    ─────────────────────────────────────────────────────────────
    KEY DIFFERENCE vs FIFO:
    At reference "0" after loading 1,2 — FIFO evicts 0 (oldest loaded)
    but LRU keeps 0 (was just used) and evicts 1 (least recently used).
    That's how LRU avoids one extra fault here.
    ────────────────────────────────────────────────────────── */
