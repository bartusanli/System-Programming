/* dosya adı : sndloop.c */
/* Kullanım: program_adı  anahtar tip  boşluksuz_bir_mesaj */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h> // sleep için

#define MAX_MESAJ 20
#define BEKLEME_SURESI 5 // Saniye

struct msgbuf { 
    long mtype;
    char mtext [100]; 
}; 

int main (int argc, char * argv[])
{
    int msid, v;
    struct msgbuf mess;
    int gonderilen_mesaj_sayisi = 0;
    
    // argüman kontrolü (key, tip, text)
    if (argc != 4) {
        printf ("Kullanım: <key> <tip> <text>\n"); 
        exit (1);
    } 

    // Mesaj kuyruğunu yaratma veya erişim sağlama 
    msid = msgget ((key_t) atoi (argv[1]), IPC_CREAT | 0666);
    if (msid == -1) {
        printf("HATA: Mesaj kuyruğunu elde edemedi\n");
        exit (1);
    }

    // Mesaj tipini ve metnini hazırla
    mess.mtype = atoi (argv[2]);
    char* base_text = argv[3];
    char tam_mesaj[150]; // Mesaj metni + sayaç için yeterli alan
    
    printf("Gönderici başladı. Anahtar: %s, Tip: %s. %d mesaj gönderecek.\n", argv[1], argv[2], MAX_MESAJ);

    // 20 mesajı kuyruğa atacak döngü
    while (gonderilen_mesaj_sayisi < MAX_MESAJ) {
        
        gonderilen_mesaj_sayisi++;
        
        // Mesaj metnini güncelleyelim (sayaç ekleyelim)
        sprintf(tam_mesaj, "%s #%d", base_text, gonderilen_mesaj_sayisi);
        strcpy(mess.mtext, tam_mesaj);
        
        // Mesajı kuyruğa gönder
        // strlen(tam_mesaj) + 1: Metin uzunluğu + null sonlandırma karakteri
        v = msgsnd (msid, &mess, strlen(tam_mesaj) + 1, 0); 

        if (v < 0) {
            perror("HATA: Mesaj kuyruğuna yazılamadı");
            break; // Hata durumunda döngüden çık
        }
        
        printf ("[%ld] Mesaj %d/%d gönderildi: %s\n", mess.mtype, gonderilen_mesaj_sayisi, MAX_MESAJ, tam_mesaj);
        
        // 5 saniye bekle
        if (gonderilen_mesaj_sayisi < MAX_MESAJ) {
            sleep(BEKLEME_SURESI);
        }
    }
    
    printf ("Gönderici sonlandı. Toplam %d mesaj gönderildi.\n", gonderilen_mesaj_sayisi); 
    return 0;
}
