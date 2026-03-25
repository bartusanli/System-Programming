/* simple.c */
/* child process için basit bir program  */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("YENİ PROGRAM %s CHILD PROCESSCE BAŞLATILDI\n", argv[0]);
    printf("Child: my ID = %d\n", getpid());
    printf("Child: my parent ID = %d\n", getppid());
    printf("Child: sonlanıyor...\n");
    exit(0);
}
