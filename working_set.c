#include <stdio.h>
#include <stdlib.h>

int main() {
    char *arr = malloc(1024*1024*50);

    for(int i=0;i<1024*1024*50;i+=4096)
        arr[i] = 1;

    printf("Working set accessed\n");
    return 0;
}
