/* consumer.c */
#include "common.h"

/* DENEY ADIM 2 İÇİN BU DEĞERİ 10 YAPACAĞIZ */
#define MAXSLEEPTIME 5 

int main(int argc, char *argv[]) {
    static char local[SLOT_LEN];
    static struct MEMORY *memptr;
    char *shmptr;
    int myparid, semid, shmid, j, sleeptime, maxmsg, curmsg;

    myparid = getppid();
    maxmsg = atoi(argv[1]);
    printf("TUKETICI basladi...\n");
    srand((unsigned)getpid());

    /* Paylaşılan belleğe eriş */
    shmid = shmget(myparid, 0, 0);
    if (shmid < 0) { perror("Tuketici: shmget hata"); exit(1); }

    shmptr = shmat(shmid, 0, 0);
    if (shmptr == (char *) -1) { perror("Tuketici: shmat hata"); exit(1); }

    memptr = (struct MEMORY *) shmptr;

    /* Semaforlara eriş */
//    semid = semget(myparid, 2, 0);
//    if (semid == -1) { perror("Tuketici: semget hata"); exit(1); }

    /* Ana Döngü */
    for (curmsg = 0; curmsg < maxmsg; curmsg++) {
        memset(local, '\0', sizeof(local));
        sleeptime = rand() % MAXSLEEPTIME;
        sleep(sleeptime);

        /* KRİTİK BÖLÜM BAŞLANGICI */
        /* Tüketilecek veri var mı bekle (Wait) */
 //       acquire.sem_num = TO_CONSUME;
 //       if (semop(semid, &acquire, 1) == -1) { perror("semop acquire hata"); exit(1); }

        /* Veriyi al */
        strcpy(local, memptr->buffer[memptr->head]);
        memptr->head = (memptr->head + 1) % N_SLOTS;

        /* Üreticiye yer açıldığını haber ver (Signal) */
 //       release.sem_num = AVAIL_SLOTS;
 //       if (semop(semid, &release, 1) == -1) { perror("semop release hata"); exit(1); }
        /* KRİTİK BÖLÜM BİTİŞİ */

        printf("...........................Tuketici mesaji aldi: %s\n", local);
    }

    printf("Tuketici sonlandi.\n");
    exit(0);
}
