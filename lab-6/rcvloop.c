/* dosya adı : rcvloop.c */
/* Kullanım: program_adı  anahtar */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h> // sleep için (isteğe bağlı, okuyucunun yavaşlamasını engeller)

struct msgbuf { 
    long mtype;
    char mtext [100]; 
}; 

int main (int argc, char * argv[])
{
    int msid, v;
    struct  msgbuf mess;
    long istenen_tip = 0; // msgrcv'de tip=0, kuyruktaki sıradaki mesajı alır.
    int alinan_mesaj_sayisi = 0;

    // argüman kontrolü (sadece key)
    if (argc != 2) {
        printf ("Kullanım: <key>\n"); 
        exit (1);
    } 

    // Mesaj kuyruğu handle'ını al (yaratma yok)
    msid = msgget ((key_t) atoi (argv[1]), 0);
    if (msid == -1) {
        printf ("Bu key (%s) ile mesaj kuyruğuna erişim sağlanamadı.\n", argv[1]);
        exit (1);
    }

    printf("Alıcı başladı. Kuyruktan tüm mesajları (tip=0) alıyor...\n");

    // Kuyruktan tüm mesajları alacak döngü
    while (1) {
        // msgrcv: 0 tipi ile sıradaki mesajı al. IPC_NOWAIT: mesaj yoksa bekleme
        v = msgrcv(msid, (struct msgbuf *)&mess, 100, istenen_tip, IPC_NOWAIT);
        
        if (v < 0) {
            // Hata kodu ENOMSG ise, kuyrukta bekleyen mesaj kalmamış demektir.
            if (errno == ENOMSG) {
                printf("Belirtilen tipte/sıradaki mesaj kuyrukta yok. Tüm mesajlar alındı.\n");
                break; // Döngüden çık
            } else {
                perror ("HATA: Kuyruktan okunamadı");
                break; // Diğer hatalarda döngüden çık
            }
        }
        
        alinan_mesaj_sayisi++;
        printf ("Alındı (%d): [%ld] %s\n", alinan_mesaj_sayisi, mess.mtype, mess.mtext);
        
        // Okuma işleminin hızını yavaşlatmak (isteğe bağlı, gözlem için yararlı olabilir)
        // usleep(100000); // 100 milisaniye bekle
    }

    // Tüm mesajlar alınınca mesaj kuyruğunu sistemden sil 
    if (msgctl(msid, IPC_RMID, 0) < 0) {
        perror("Hata: Mesaj kuyruğu sistemden silinemedi");
        exit(1);
    } else {
        printf("Mesaj kuyruğu (key = %s) sistemden silindi.\n", argv[1]);
    }
    
    printf("Alıcı sonlandı. Toplam %d mesaj alındı.\n", alinan_mesaj_sayisi);
    return 0;
}
