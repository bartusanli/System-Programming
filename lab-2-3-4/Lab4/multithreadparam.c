/* multithreadparam.c (kilitsiz - yarış koşulu örneği) */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

int r1 = 0, r2 = 0;

struct pair {
  int *r1p;
  int *r2p;
};

void *worker(void *arg)
{
  struct pair *p = (struct pair *)arg;
  /* sleepten önce r1'i bir arttır */
  (*(p->r1p))++;

  /* sleep ile simüle et */
  sleep(1);

  /* sleepten sonra r2'yi bir arttır */
  (*(p->r2p))++;

  return NULL;
}

int main(void)
{
  pthread_t td1, td2;
  struct pair p;
  p.r1p = &r1;
  p.r2p = &r2;

  /* ana thread de r1/r2'yi arttırsın (direkt) */
  r1++;   /* ana thread sleepten önce bir kez arttır */
  /* burada istersen sleep yap */
  sleep(1);
  r2++;   /* ana thread sleepten sonra bir kez arttır */

  /* iki child thread yarat */
  pthread_create(&td1, NULL, worker, &p);
  pthread_create(&td2, NULL, worker, &p);

  pthread_join(td1, NULL);
  pthread_join(td2, NULL);

  printf("r1 = %d, r2 = %d\n", r1, r2);

  return 0;
}
