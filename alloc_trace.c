#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    void *brk_before = sbrk(0);
    printf("Initial break: %p\n\n", brk_before);

    char *small = malloc(1024);
    printf("After malloc(1KB): break=%p ptr=%p\n", sbrk(0), small);

    char *large = malloc(256 * 1024);
    printf("After malloc(256KB): break=%p ptr=%p\n", sbrk(0), large);

    char *small2 = malloc(512);
    printf("After malloc(512B): break=%p ptr=%p\n", sbrk(0), small2);

    printf("\nsmall  at %p\n", small);
    printf("small2 at %p\n", small2);
    printf("large  at %p\n", large);

    return 0;
}
