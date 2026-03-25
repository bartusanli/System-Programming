/* dosya adı : rcvstruct.c */
/* Kullanım: program_adı  anahtar tip */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

// Öğrenci bilgilerini tutan yapı
typedef struct {
    int ogrenci_no;
    char ad[30];
    char sehir[30];
} tbilgi;

// Mesaj kuyruğu yapısı
struct msgbuf { 
    long mtype;
    char mtext [sizeof(tbilgi) + 1]; // Struct'ı tutmak için yeterli boyut
}; 

int main (int argc, char * argv[])
{
    int msid, v;
    struct msgbuf mess;
    long istenen_tip;

    if (argc != 3) {
        printf ("Kullanım: <key> <tip>\n"); 
        exit (1);
    } 

    istenen_tip = atoi(argv[2]);

    // 1. Mesaj kuyruğu handle'ını al
    msid = msgget ((key_t) atoi (argv[1]), 0);
    if (msid == -1) {
        printf ("Bu key ile mesaj kuyruğuna erişim sağlanamadı. (Belki gönderici henüz yaratmadı?)\n");
        exit (1);
    }

    // 2. Kuyruktan mesaj al
    // msgrcv'deki boyut (sizeof(mess.mtext)), mesaj tamponunun alabileceği maksimum boyuttur.
    v = msgrcv(msid, (struct msgbuf *)&mess, sizeof(mess.mtext), istenen_tip, IPC_NOWAIT);
    
    if (v < 0) {
        if (errno == ENOMSG) {
            printf("Belirtilen tipte mesaj kuyrukta yok.\n");
        } else {
            perror ("HATA: Kuyruktan okunamadı"); 
        }
    } else {
        printf("Mesaj alındı. Tip: [%ld], Boyut: %d byte\n", mess.mtype, v);
        
        // 3. Mesaj tipine göre içeriği yorumla
        if (mess.mtype == 1) { 
            // TIP 1: Öğrenci Bilgisi
            if (v == sizeof(tbilgi)) {
                tbilgi alinan_bilgi;
                // Gelen veriyi struct'a kopyala
                memcpy(&alinan_bilgi, mess.mtext, sizeof(tbilgi));
                
                printf("--- Öğrenci Bilgisi ---\n");
                printf("No: %d\n", alinan_bilgi.ogrenci_no);
                printf("Ad: %s\n", alinan_bilgi.ad);
                printf("Şehir: %s\n", alinan_bilgi.sehir);
                printf("------------------------\n");
            } else {
                 printf("HATA: Beklenen Öğrenci Yapısı Boyutu Eşleşmedi.\n");
            }

        } else if (mess.mtype == 2) { 
            // TIP 2: Metin Mesajı
            printf("--- Metin Mesajı ---\n");
            printf("İçerik: %s\n", mess.mtext);
            printf("--------------------\n");

        } else {
            printf("Bilinmeyen Mesaj Tipi.\n");
        }
    }

    // 4. Mesaj kuyruğunu sistemden sil
    if (msgctl(msid, IPC_RMID, 0) < 0) {
        perror("Hata: Mesaj kuyruğu sistemden silinemedi");
        exit(1);
    } else {
        printf("Mesaj kuyruğu (key = %s) sistemden silindi.\n", argv[1]);
    }
    
    return 0;
}
