#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MB (1024 * 1024)
#define ALLOC_MB 128

static long get_minor_faults(void) {
    // Field 10 in /proc/self/stat is minflt (minor page faults)
    FILE *f = fopen("/proc/self/stat", "r");
    if (!f) return -1;

    char line[4096];
    if (!fgets(line, sizeof(line), f)) { fclose(f); return -1; }
    fclose(f);

    // Skip past the comm field "(name)" which may contain spaces
    char *p = strrchr(line, ')');
    if (!p) return -1;
    p += 2;  // skip ") "

    // Parse: state, ppid, pgrp, session, tty, tpgid, flags, minflt
    long minflt;
    char state;
    int dummy;
    unsigned long flags;
    sscanf(p, "%c %d %d %d %d %d %lu %ld",
           &state, &dummy, &dummy, &dummy, &dummy, &dummy, &flags, &minflt);
    return minflt;
}

int main(void) {
    char *region = malloc(ALLOC_MB * MB);
    memset(region, 'P', ALLOC_MB * MB);  // parent touches all pages

    printf("Parent: allocated and touched %d MB (%d pages)\n",
           ALLOC_MB, ALLOC_MB * 256);

    pid_t pid = fork();

    if (pid == 0) {
        // ---- CHILD ----
        long faults_start = get_minor_faults();

        // READ-ONLY pass: pages are shared, shouldn't trigger COW
        volatile char sink = 0;
        for (int i = 0; i < ALLOC_MB * MB; i += 4096)
            sink += region[i];
        long faults_after_read = get_minor_faults();

        printf("\nChild READ-ONLY scan:\n");
        printf("  Page faults: %ld  (should be ~0, pages are shared)\n",
               faults_after_read - faults_start);

        // WRITE pass: every page triggers a COW fault
        long faults_before_write = get_minor_faults();
        memset(region, 'C', ALLOC_MB * MB);
        long faults_after_write = get_minor_faults();

        printf("\nChild WRITE pass:\n");
        printf("  Page faults: %ld\n",
               faults_after_write - faults_before_write);
        printf("  Expected:    %d  (one COW fault per page)\n",
               ALLOC_MB * 256);

        free(region);
        _exit(0);
    }

    // ---- PARENT ----
    wait(NULL);
    free(region);
    return 0;
}
