/* func1.c */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void func1(void)
{
  int i;
  for (i = 1; i < 10; ++i)
  {
    printf("Fonksiyon func1 yazar ve 4 saniye uyur: %d/10 \n", i);
    sleep(4);
  }
  return;
}
//Paket açma
void *func1_thread(void *arg)
{
  (void)arg;
  func1();           /* aynı işi yapar */
  return NULL;
}
