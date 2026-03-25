/* dosya adı : sndstruct.c */
/* Kullanım:
 * TIP 1 (Öğrenci): sndstruct <key> 1 <no> <ad> <sehir>
 * TIP 2 (Metin):   sndstruct <key> 2 <text>
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

// Öğrenci bilgilerini tutan yapı
typedef struct {
    int ogrenci_no;
    char ad[30];
    char sehir[30];
} tbilgi;

// Mesaj kuyruğu yapısı: mtext alanı en büyük veri yapısını (tbilgi) alacak büyüklükte olmalı.
struct msgbuf { 
    long mtype;
    char mtext [sizeof(tbilgi) + 1]; // Struct'ı tutmak için yeterli boyut
}; 

int main (int argc, char * argv[])
{
    int msid, v;
    struct msgbuf mess;
    long mesaj_tipi;
    size_t mesaj_boyutu;

    // Argüman kontrolü
    if (argc < 4 || argc > 6) {
        printf ("Hatalı Kullanım!\n");
        printf ("TIP 1 (Öğrenci): %s <key> 1 <no> <ad> <sehir>\n", argv[0]);
        printf ("TIP 2 (Metin):   %s <key> 2 <text>\n", argv[0]);
        exit (1);
    } 

    mesaj_tipi = atoi(argv[2]);
    
    // 1. Mesaj kuyruğunu yaratma veya erişim sağlama 
    msid = msgget ((key_t) atoi (argv[1]), IPC_CREAT | 0666);
    if (msid == -1) {
        perror("HATA: Mesaj kuyruğunu elde edemedi");
        exit (1);
    }

    mess.mtype = mesaj_tipi;

    // 2. Mesaj tipine göre veriyi hazırla
    if (mesaj_tipi == 1 && argc == 6) { 
        // TIP 1: Öğrenci Bilgisi
        tbilgi ogrenci_bilgisi;
        
        ogrenci_bilgisi.ogrenci_no = atoi(argv[3]);
        strncpy(ogrenci_bilgisi.ad, argv[4], sizeof(ogrenci_bilgisi.ad) - 1);
        ogrenci_bilgisi.ad[sizeof(ogrenci_bilgisi.ad) - 1] = '\0';
        strncpy(ogrenci_bilgisi.sehir, argv[5], sizeof(ogrenci_bilgisi.sehir) - 1);
        ogrenci_bilgisi.sehir[sizeof(ogrenci_bilgisi.sehir) - 1] = '\0';
        
        // struct'ı mesaj tamponuna kopyala (char* tipine cast edilip)
        memcpy(mess.mtext, &ogrenci_bilgisi, sizeof(tbilgi));
        mesaj_boyutu = sizeof(tbilgi);

        printf("TIP 1 (Öğrenci) Mesajı Hazırlandı.\n");

    } else if (mesaj_tipi == 2 && argc == 4) {
        // TIP 2: Metin Mesajı
        strncpy(mess.mtext, argv[3], sizeof(mess.mtext) - 1);
        mess.mtext[sizeof(mess.mtext) - 1] = '\0';
        mesaj_boyutu = strlen(mess.mtext) + 1; // Null sonlandırma ile metin boyutu

        printf("TIP 2 (Metin) Mesajı Hazırlandı.\n");
        
    } else {
        printf ("HATA: Eksik veya yanlış argümanlar.\n");
        exit (1);
    }
    
    // 3. Mesajı kuyruğa gönder
    v = msgsnd (msid, &mess, mesaj_boyutu, 0); 

    if (v < 0) {
        perror("HATA : Mesaj kuyruğuna yazılamadı"); 
    } else {
        printf ("[%ld] Mesaj başarıyla gönderildi. Boyut: %zu byte\n", mess.mtype, mesaj_boyutu);
    }
    
    printf ("Gönderici sonlandı.\n"); 
    return 0;
}
