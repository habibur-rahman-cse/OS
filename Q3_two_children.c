#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1 = fork();

    if (pid1 < 0) {
        printf("Fork #1 failed!\n");
        return 1;
    }

    if (pid1 == 0) {
        printf("\n[Child 1]\n");
        printf("  PID  (my own)      = %d\n", getpid());
        printf("  PPID (my parent)   = %d\n", getppid());
        printf("  I am the FIRST child.\n");
        return 0;
    }

    pid_t pid2 = fork();
    
    if (pid2 < 0) {
        printf("Fork #2 failed!\n");
        return 1;
    }

    if (pid2 == 0) {
        printf("\n[Child 2]\n");
        printf("  PID  (my own)      = %d\n", getpid());
        printf("  PPID (my parent)   = %d\n", getppid());
        printf("  I am the SECOND child.\n");
        return 0;  // child 2 exits here
    }

    printf("\n[Parent]\n");
    printf("  PID  (my own)      = %d\n", getpid());
    printf("  Child 1 PID        = %d\n", pid1);
    printf("  Child 2 PID        = %d\n", pid2);

    // Print process tree
    printf("\n  Process Tree:\n");
    printf("  Parent (%d)\n", getpid());
    printf("  ├── Child 1 (%d)\n", pid1);
    printf("  └── Child 2 (%d)\n", pid2);

    wait(NULL);   // waits for first child to finish
    wait(NULL);   // waits for second child to finish
    printf("\n[Parent] Both children have finished. Exiting.\n");

    return 0;
}

/*  ── SAMPLE OUTPUT ───────────────────────────────────────────
    [Parent]
      PID  (my own)      = 1000
      Child 1 PID        = 1001
      Child 2 PID        = 1002

      Process Tree:
      Parent (1000)
      ├── Child 1 (1001)
      └── Child 2 (1002)

    [Child 1]
      PID  (my own)      = 1001
      PPID (my parent)   = 1000
      I am the FIRST child.

    [Child 2]
      PID  (my own)      = 1002
      PPID (my parent)   = 1000
      I am the SECOND child.

    [Parent] Both children have finished. Exiting.

    ── WHY return 0 after each child? ──────────────────────────
    Without it, Child 1 would also run fork() #2 and create
    its own child — giving you 4 processes instead of 3!
    Always exit children before they reach the next fork().
    ────────────────────────────────────────────────────────── */
