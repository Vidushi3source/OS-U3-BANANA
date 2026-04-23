#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Fragmentation demo\n");

    char *a = malloc(10);
    char *b = malloc(20);
    char *c = malloc(30);

    printf("Addresses: %p %p %p\n", a, b, c);

    return 0;
}
