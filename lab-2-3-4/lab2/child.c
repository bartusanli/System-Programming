/* child.c */
/* mainprog.c için child */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int ret_value;

    pid = getpid();
    ret_value = (int)(pid % 256);

    printf("Child %d status = %02X ile dönecek\n", pid, ret_value);
    srand((unsigned)pid);
    sleep(rand() % 5); /* 0 - 4 s arasında gelişigüzel bir zaman uyu */

    if (atoi(argv[1]) % 2) {
        printf("Child %d SIGNAL 0009 ile sonlanıyor\n", pid);
        kill(pid, 9); /* child kendini öldürüyor */
    } else {
        printf("Child %d exit(%04X) ile sonlanıyor\n", pid, ret_value);
        exit(ret_value);
    }
}
