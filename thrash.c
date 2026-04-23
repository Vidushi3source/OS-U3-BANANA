#include <stdio.h>
#include <stdlib.h>

int main() {
    char *arr = malloc(1024L*1024*200);

    for(long i=0;i<1024L*1024*200;i+=4096)
        arr[i] = i;

    printf("Thrashing simulation done\n");
    return 0;
}
