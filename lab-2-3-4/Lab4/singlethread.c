#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
 
/* Fonk. Tanımları */
 void func1(void);
 void func2(void); 

int r1 = 0, r2 = 0; /* Global variables, gerekli olduğunda fonksiyonlar kullanacak*/

int 
main(void)
{
  pthread_t td1, td2; /* 2 child thread için tanımlayıcılar*/
  int p; /* burada kullanılmıyor */ 
  int j;
  float delta;
  struct timeval time1, time2;

  printf("Tek-Threadli process başlıyor...\n");
   gettimeofday(&time1, 0); /* Başlama zamanı */

/* 2 child thread yaratılması */
  p = pthread_create(&td1,NULL,(void *)func1, NULL); 

  if (p != 0) {perror("Thread 1 creation problem"); exit(1);}

  p = pthread_create(&td2,NULL,(void *)func2,NULL);

  if (p != 0) {perror("Thread 2 creation problem"); exit(1);} 
          
/* şimdi tek process, tek ana threadi ile bir şeyler yapıyor */ 
  for (j=1; j<=4; ++j)
    {
    printf("Processin ana fonksiyonu: %d/4\n", j);
    sleep(3); /* 3 saniye uyu */
    }
   
/* Tüm processler sıra ile iki fonksiyonu çağırıyorlar */
  func1();
  func2();  

/* Processin toplam çalışma zamanlarını hesaplama */
   gettimeofday (&time2, 0);
   delta = (float)((1000000*time2.tv_sec + time2.tv_usec) -
                   (1000000*time1.tv_sec + time1.tv_usec))/1000000;
   printf("Toplam geçen zaman = %f saniye\n", delta);

   printf("Ana thread sonlandı...\n");

   return 0; 
}
