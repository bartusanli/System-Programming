/* common.h */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define MSGNUMBER 10           /* Tüketicideki farklı mesaj sayısı */
#define SLOT_LEN  50           /* Bir mesajdaki max karakter sayısı */
#define N_SLOTS   6            /* Kuyruktaki max boş slot sayısı (Buffer boyutu) */
#define MAXMSGS   25           /* Üretilen ya da tüketilen max mesaj sayısı */

/* Paylaşılmış bellek yapısı */
struct MEMORY {
    char buffer[N_SLOTS][SLOT_LEN];  /* Mesajlar için array (Dairesel Kuyruk) */
    int head, tail;                  /* Kuyruğun başı ve sonu için indeksler */
};

/* Semafor operasyonları için struct tanımları */
struct sembuf acquire = {0, -1, SEM_UNDO};  /* Kaynak al (P işlemi / Wait) */
struct sembuf release = {0,  1, SEM_UNDO};  /* Kaynak bırak (V işlemi / Signal) */

/* Semafor indeksleri */
enum {AVAIL_SLOTS, TO_CONSUME}; 
/* AVAIL_SLOTS (0): Boş slot sayısını tutar.
   TO_CONSUME (1): Dolu (tüketilecek) slot sayısını tutar.
*/
