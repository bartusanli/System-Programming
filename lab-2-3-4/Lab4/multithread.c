/* multithread.c */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

/* Fonk. Tanımları (func1.c/func2.c içinde wrapper'lar tanımlı) */
void *func1_thread(void *arg);
void *func2_thread(void *arg);

int r1 = 0, r2 = 0;

int main(void)
{
  pthread_t td1, td2;
  int p;
  int j;
  float delta;
  struct timeval time1, time2;

  printf("Çok-threadli process ana thread olarak başlıyor...\n");
  gettimeofday(&time1, 0);

  /* 2 child thread yaratılması */
  //p = pthread_create(&td1, NULL, func1_thread, NULL);
  //if (p != 0) { perror("Thread 1 creation problem"); exit(1); }

  //p = pthread_create(&td2, NULL, func2_thread, NULL);
  //if (p != 0) { perror("Thread 2 creation problem"); exit(1); }

  /* Şimdi ana thread bir şeyler yapıyor */
  for (j=1; j<=4; ++j)
  {
    printf("Ana thread çalışıyor: %d/4\n", j);
    sleep(3);
  }

  pthread_join(td1, NULL);
  pthread_join(td2, NULL);

  gettimeofday(&time2, 0);
  delta = (float)((1000000*time2.tv_sec + time2.tv_usec) -
                  (1000000*time1.tv_sec + time1.tv_usec))/1000000;

  printf("Toplam geçen zaman = %f saniye\n", delta);
  printf("Ana thread sonlandı...\n");
  return 0;
}
