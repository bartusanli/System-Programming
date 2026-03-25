/* Switching to another program for a child process */
/* Investigation of wait() and exit() system calls */
/* See also the file child.c */
/* Adapted from J.S.Gray, pp. 73 -74 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main () //duzeltme
{
pid_t pid,w;
int i,status;
char value[3]; /* A place to store index as a string */
for (i=0; i<3;++i)
    { 
    /* Parent creates 3 child processes */
    if ((pid = fork()) ==0)
        {
        sprintf(value, "%d", i); /* value gets 0, 1, or 2 */
        /* Each child switches to the program child
           with argument value, without return */
        execl("./child", "child", value, NULL); //duzeltme 
        }
    else /* Parent continues here */ 
    printf("Parent created child ID = %d\n", pid);
    }

/* Parent waits when all its children  terminate */
/* w = child ID  or  -1 if there is no child any more 
*/
while ((w = wait(&status)) && w != -1)
   {
   if (w != -1) /* w = -1,  no more children to wait */
   printf ("Parent: child ID = %d returned status =  %04X\n",
           w, status);
   }
exit(0);
}
