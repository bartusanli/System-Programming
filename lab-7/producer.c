/* producer.c */
#include "common.h"

int main(int argc, char *argv[]) {
    static char *messages[MSGNUMBER] = {
        "First message", "Second message", "Third message", "Fourth message",
        "Fifth message", "Sixth message", "Seventh message", "Eighth message",
        "Ninth message", "Tenth message"
    };
    
    static char local[SLOT_LEN];
    static struct MEMORY *memptr;
    char *shmptr;
    int myparid, semid, shmid, i, j, sleeptime, maxmsg, curmsg;

    myparid = getppid(); /* Parent ID'yi al (Key olarak kullanılacak) */
    maxmsg = atoi(argv[1]);
    printf("URETICI basladi... (Uretilecek: %d)\n", maxmsg);
    srand((unsigned)getpid());

    /* Paylaşılan belleğe eriş */
    shmid = shmget(myparid, 0, 0);
    if (shmid < 0) { perror("Uretici: shmget hata"); exit(1); }
    
    shmptr = shmat(shmid, 0, 0);
    if (shmptr == (char *) -1) { perror("Uretici: shmat hata"); exit(1); }
    
    memptr = (struct MEMORY *) shmptr;
    
    /* Semaforlara eriş */
//    semid = semget(myparid, 2, 0);
//    if (semid == -1) { perror("Uretici: semget hata"); exit(1); }

    /* Ana Döngü */
    for (curmsg = 0; curmsg < maxmsg; curmsg++) {
        memset(local, '\0', sizeof(local));
        sleeptime = rand() % 6; /* 0-5 sn bekle */
        sleep(sleeptime);

        i = curmsg % MSGNUMBER;
        strcpy(local, messages[i]); /* Mesajı hazırla */
        
        printf("Uretici hazirladi: %s\n", local);

        /* KRİTİK BÖLÜM BAŞLANGICI */
        /* Boş slot var mı diye kontrol et (Wait) */
//        acquire.sem_num = AVAIL_SLOTS;
//        if (semop(semid, &acquire, 1) == -1) { perror("semop acquire hata"); exit(1); }

        /* Mesajı belleğe yaz */
        strcpy(memptr->buffer[memptr->tail], local);
        memptr->tail = (memptr->tail + 1) % N_SLOTS; /* Dairesel kuyruk mantığı */

        /* Tüketiciye tüketilecek veri olduğunu haber ver (Signal) */
  //      release.sem_num = TO_CONSUME;
  //      if (semop(semid, &release, 1) == -1) { perror("semop release hata"); exit(1); }
        /* KRİTİK BÖLÜM BİTİŞİ */
    }

    printf("Uretici sonlandi.\n");
    exit(0);
}
