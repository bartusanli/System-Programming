/* procident.c */
/* Process ID (PID) çalışması */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {  
    int i = 0;   
    pid_t pid, parent, child, myparent;

    pid = fork(); /* Parent bir child process yaratır */

    if (pid == 0) { /* sadece child için */
        child = getpid(); 
        i = 2;
        printf("Child: my ID = %d, i = %d\n", child, i);
        myparent = getppid();
        printf("Child: my parent ID = %d\n", myparent);
        sleep(3); /* Child 10 saniye uyur */
        child = getpid();
        printf("\nChild uyuduktan sonra: my ID = %d\n", child);
        myparent = getppid();
        printf("Child uyuduktan sonra: my parent ID = %d\n", myparent);
        execl("./simple", "simple", NULL); /* simple programına geçiş */
        perror("execl failed");
        exit(1);
    } 
    else {     
        sleep(10); 
        parent = getpid(); 
        printf("Parent: my ID = %d, i = %d\n",  parent, i);
        printf("Parent: sonlanıyor...\n");
    }
}
