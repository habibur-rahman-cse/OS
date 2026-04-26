// ============================================================
//  Q2 — Parent prints 1–5, Child prints 6–10
//
//  Key point: after fork(), BOTH processes run the code below
//  the fork() call. The if/else separates their behaviour.
//  Output order is NOT guaranteed — OS schedules freely.
// ============================================================
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed!\n");
        return 1;

    } else if (pid == 0) {
        // ── CHILD: prints 6 to 10 ──
        printf("[Child  - PID %d] Starting...\n", getpid());
        for (int i = 6; i <= 10; i++) {
            printf("[Child  - PID %d] %d\n", getpid(), i);
            sleep(1);  // slow it down to observe interleaving
        }
        printf("[Child  - PID %d] Done.\n", getpid());

    } else {
        // ── PARENT: prints 1 to 5 ──
        printf("[Parent - PID %d] Starting...\n", getpid());
        for (int i = 1; i <= 5; i++) {
            printf("[Parent - PID %d] %d\n", getpid(), i);
            sleep(1);  // slow it down to observe interleaving
        }
        printf("[Parent - PID %d] Waiting for child...\n", getpid());
        wait(NULL);   // parent waits for child to finish
        printf("[Parent - PID %d] Child finished. Parent exits.\n", getpid());
    }

    return 0;
}

/*  ── SAMPLE OUTPUT (interleaved — varies each run) ───────────
    [Parent - PID 1000] Starting...
    [Child  - PID 1001] Starting...
    [Parent - PID 1000] 1
    [Child  - PID 1001] 6
    [Parent - PID 1000] 2
    [Child  - PID 1001] 7
    [Parent - PID 1000] 3
    [Child  - PID 1001] 8
    [Parent - PID 1000] 4
    [Child  - PID 1001] 9
    [Parent - PID 1000] 5
    [Child  - PID 1001] 10
    [Parent - PID 1000] Waiting for child...
    [Child  - PID 1001] Done.
    [Parent - PID 1000] Child finished. Parent exits.

    Remove sleep() and the output will be much more interleaved!
    ────────────────────────────────────────────────────────── */