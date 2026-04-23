#include <stdio.h>
#include <stdlib.h>

int main() {
    char *arr = malloc(1024*1024*100);
    for(int i=0;i<1024*1024*100;i+=4096)
        arr[i] = i;

    printf("Memory accessed\n");
    return 0;
}
