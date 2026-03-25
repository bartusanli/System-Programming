
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {

pid_t pid;

char *args[4];


if ((pid = fork()) < 0) {

perror("fork failed");

exit(1);

}


if (pid == 0) {

execl("/bin/echo", "echo", "Today's", "date", "is:", NULL);


/*

* eğer exec başarılı ise buraya gelmeyecektir.

*/

perror("exec failed in child");

exit(1);

}


/*

* Bu kodu sadece parent işletecek

*/

/* burada execv için parametreleri hazırlıyor ..execv exec ailesinin başka bir üyesi*/

args[0] = "date";

args[1] = "+Date = %D Time = %H:%M";

args[2] = NULL;


execv("/bin/date", args);

/*

* eğer exec başarılı ise buraya gelmeyecektir..

*/

perror("exec failed in parent");

exit(1);

}
