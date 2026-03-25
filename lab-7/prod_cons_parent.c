/* prod_cons_parent.c */
#include "common.h"

int main(int argc, char *argv[]) {
    static struct MEMORY sharmemory;
    static ushort init_val[2] = {N_SLOTS, 0}; /* AVAIL_SLOTS = 6 (Başlangıçta hepsi boş)
                                                TO_CONSUME = 0 (Başlangıçta tüketilecek yok)
                                              */
    int semid, shmid, child;
    char *shmptr;
    int prod_id, cons_id, parent_id;
    union semun {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } arguments;

    printf("Parent process basliyor...\n");

    if (argc != 2) {
        printf("Kullanim: %s mesaj_sayisi\n", argv[0]);
        exit(1);
    }

    /* 1. Paylaşılmış Belleği Yarat ve Bağla */
    parent_id = getpid();
    shmid = shmget(parent_id, sizeof(sharmemory), IPC_CREAT | 0666);
    if (shmid < 0) { perror("shmget problem"); exit(1); }
    
    shmptr = shmat(shmid, 0, 0);
    if (shmptr == (char *) -1) { perror("shmat problem"); exit(1); }
    
    /* Belleği sıfırla */
    sharmemory.head = sharmemory.tail = 0;
    memcpy(shmptr, (char *)&sharmemory, sizeof(sharmemory));
    printf("Paylasilmis Bellek hazir...\n");

    /* 2. Semaforları Yarat ve İlklendir */
    semid = semget(parent_id, 2, IPC_CREAT | 0666);
    if (semid == -1) { perror("semget problem"); exit(1); }
    
    arguments.array = init_val;
    if (semctl(semid, 0, SETALL, arguments) == -1) { perror("semctl problem"); exit(1); }
    printf("Semaphorlar hazir...\n");

    /* 3. Child Prosesleri Başlat */
    prod_id = fork();
    if (prod_id == 0) execl("./producer", "producer", argv[1], NULL);

    cons_id = fork();
    if (cons_id == 0) execl("./consumer", "consumer", argv[1], NULL);

    /* 4. Bekle ve Temizle */
    printf("Parent process cocuklari bekliyor...\n");
    wait(NULL);
    wait(NULL);

    /* Belleği ve Semaforu Sil */
    shmdt(shmptr);
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID, 0);
    
    printf("\nTum processler sonlandi ve temizlik yapildi.\n");
    exit(0);
}
