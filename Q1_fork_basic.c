// ============================================================
//  Q1 — Basic fork(): Print Parent PID, Child PID, Child's PPID
//
//  Memory trick:
//    fork() > 0  →  I am the PARENT  (got child's PID back)
//    fork() == 0 →  I am the CHILD   (got zero back)
// ============================================================
#include <stdio.h>
#include <unistd.h>    // fork(), getpid(), getppid()
#include <sys/wait.h>  // wait()

int main() {
    pid_t pid = fork();  // duplicate this process

    if (pid < 0) {
        printf("Fork failed!\n");
        return 1;

    } else if (pid == 0) {
        printf("\n[CHILD]  My PID           = %d\n", getpid());
        printf("[CHILD]  My Parent's PID  = %d\n",  getppid());

    } else {
        printf("\n[PARENT] My PID           = %d\n", getpid());
        printf("[PARENT] Child's PID      = %d\n",  pid);
        wait(NULL);  // wait for child to finish before exiting
    }

    return 0;
}

/*  ── SAMPLE OUTPUT ───────────────────────────────────────────
    [PARENT] My PID           = 1000
    [PARENT] Child's PID      = 1001

    [CHILD]  My PID           = 1001
    [CHILD]  My Parent's PID  = 1000

    Note: Child's PPID always equals Parent's PID  ✓
    ────────────────────────────────────────────────────────── */
