/* func2.c */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void func2(void)
{
  int i;
  for (i = 1; i < 8; ++i)
  {
    printf("Fonksiyon func2 yazar ve 3 saniye uyur: %d/8 \n", i);
    sleep(3);
  }
  return;
//paket açma
}
void *func2_thread(void *arg)
{
  (void)arg;
  func2();
  return NULL;
}
