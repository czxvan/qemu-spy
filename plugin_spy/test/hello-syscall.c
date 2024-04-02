#include <stdio.h>
#include <unistd.h>
int main(void) {
    int ret = fork();
    if (ret == 0) {
        printf("hello from child\n");
    } else {
        printf("hello from parent\n");
    }
    return 0;
}