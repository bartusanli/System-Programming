/* dosya adı : sndmsg.c */
/* Kullanım: program_adı  anahtar tip  boşluksuz_bir_mesaj */

#include <stdio.h>
#include <stdlib.h> // exit için
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h> // strcpy için

// msgbuf veri yapısı (types.h'de tanımlı değilse gereklidir)
struct msgbuf { 
    long mtype;
    char mtext [100]; 
}; 

int main (int argc, char * argv[])
{
    int msid, v;
    struct msgbuf mess;

    if (argc != 4) {
        printf ("Kullanım: <key> <tip> <text>\n"); 
        exit (1);
    } 

    // mesaj kuyruğunu yaratma veya erişim sağlama 
    // IPC_CREAT: Kuyruk yoksa yarat, 0666: Okuma/Yazma izinleri
    msid = msgget ((key_t) atoi (argv[1]), IPC_CREAT | 0666);
    if (msid == -1) {
        printf("Mesaj kuyruğunu elde edemedi\n");
        exit (1);
    }

    // Komut satırından mesajı hazırlayalım 
    mess.mtype = atoi (argv[2]);     /* mesaj tipi */
    strcpy (mess.mtext, argv[3]);    /* mesajın metni */
    
    // mesajı kuyruğa gönder
    // strlen(argv[3]) + 1: Metin uzunluğu + null sonlandırma karakteri
    v = msgsnd (msid, &mess, strlen(argv[3]) + 1, 0); 

    if (v < 0) {
        printf("HATA : Mesaj kuyruğuna yazılamadı\n"); 
    }
    
    printf ("Gönderici sonlandı\n"); 
    return 0; // exit(0) yerine return 0 kullanıldı
}
