/* mainprog.c */ 
/* Child ‘ın başka bir programa geçişi */
/* wait() ve exit() system çağrılarının incelenmesi */
/* child.c ‘ yide görün */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main ()
{
pid_t pid,w;
int i,status;
char value[3]; /* string */
for (i=0; i<3;++i)
    { 
    /* Parent 3 child process yaratır */
    if ((pid = fork()) ==0)
        {
        sprintf(value, "%d", i); /* değer 0,1 ve 2 olabilir*/
        /* Herbir child geridönüşü olmaksızın child programına geçer */
        execl("child", "child", value, NULL); 
        }
    else /* Parent burada devam eder */ 
    printf("Parent child (ID = %d) yarattı\n", pid);
    }

/* parent,tüm child ların işini bitirmesini bekler */
/* w = child ID  veya  -1 eğer başka bekleyecek child yoksa 
*/
while ((w = wait(&status)) && w != -1)
   {
   if (w != -1) /* w = -1,  başka child yok */
   printf ("Parent: child ID = %d status =  %04X ile döndü\n",
           w, status);
   }
exit(0);
}
