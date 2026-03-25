/* multiprocess.c */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

/* func1() ve func2() doğrudan func1.c/func2.c'de olacak */
void func1(void);
void func2(void);

int main(void)
{
  pid_t pid;
  float delta;
  struct timeval time1, time2;

  printf("Multiprocess (fork) programı başlıyor...\n");
  gettimeofday(&time1, 0);

  pid = fork();
  if (pid < 0) { perror("fork failed"); exit(1); }

  if (pid == 0) {
    /* child process: örneğin func2'yi yap */
    printf("Child process: func2 çalışıyor (pid=%d)\n", getpid());
    func2();
    printf("Child bitti\n");
    exit(0);
  } else {
    /* parent process: func1 yap */
    printf("Parent process: func1 çalışıyor (pid=%d)\n", getpid());
    func1();

    /* parent işini bitirdikten sonra child için bekle */
    waitpid(pid, NULL, 0);

    gettimeofday(&time2, 0);
    delta = (float)((1000000*time2.tv_sec + time2.tv_usec) -
                    (1000000*time1.tv_sec + time1.tv_usec))/1000000;

    printf("Toplam geçen zaman (multiprocess) = %f saniye\n", delta);
    return 0;
  }
}
