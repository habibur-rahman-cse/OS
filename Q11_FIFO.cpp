#include <iostream>
#include <vector>
using namespace std;

bool isInFrames(vector<int> &frames, int page) {
    int f = frames.size();
    for (int i = 0; i < f; i++)
        if (frames[i] == page) return true;
    return false;
}

int main() {
    int n, f;
    cin >> n; // number of pages
    vector<int> pages(n);
    for (int i = 0; i < n; i++) {
        cin >> pages[i];
    }

    cin >> f; // number of frames;
    vector<int> frames(f);
    for (int i = 0; i < f; i++) {
        frames[i] = -1;
    }

    int ptr = 0;   // FIFO pointer — points to oldest page
    int pageFaults = 0;
    int pageHits = 0;

    cout << "Page\tFrames\t\tResult\n";

    for (int i = 0; i < n; i++) {
        cout << pages[i] << "\t\t";

        if (isInFrames(frames, pages[i])) {
            pageHits++;
            for (int j = 0; j < f; j++)
                cout << (frames[j]==-1 ? "-" : to_string(frames[j])) << " ";
            cout << "\t\tHIT\n";
        } else {
            pageFaults++;
            int evicted = frames[ptr];
            frames[ptr] = pages[i];
            ptr = (ptr + 1) % f;  // advance FIFO pointer

            for (int j = 0; j < f; j++)
                cout << (frames[j]==-1 ? "-" : to_string(frames[j])) << " ";
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
    2     2  0  1       FAULT (evicted 7)  ← oldest was 7
    0     2  0  1       HIT
    3     2  3  1       FAULT (evicted 0)  ← oldest was 0
    0     2  3  0       FAULT (evicted 1)  ← oldest was 1
    4     4  3  0       FAULT (evicted 2)
    2     4  2  0       FAULT (evicted 3)
    3     4  2  3       FAULT (evicted 0)
    0     0  2  3       FAULT (evicted 4)
    3     0  2  3       HIT

    Total Page Faults : 9
    Total Page Hits   : 3
    ────────────────────────────────────────────────────────── */
