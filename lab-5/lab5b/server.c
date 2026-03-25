/* A server using FIFO pipes for two-way communication */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h> // exit() için
#include <unistd.h> // unlink(), read(), write(), close() için
#include <string.h> // strcmp() için

#define MAX_BUFFER 1024
#define REPLY_MESSAGE "THIS IS A REPLY FROM SERVER"
#define BYE_MESSAGE "bye"

// Kullanım: ./server pipe_req pipe_res &
int main(int argc, char *argv[]) {
    int fd_req, fd_res;
    ssize_t n_read;
    char buf[MAX_BUFFER];
    
    if (argc < 3) {
        printf("Kullanim: %s <request_pipe_name> <response_pipe_name> &\n", argv[0]);
        exit(1);
    }
    
    printf("SERVER BASLIYOR... (PID: %d)\n", getpid());

    // 1. Önceki pipe'ları sil (Gerekliyse) [cite: 75]
    unlink(argv[1]); 
    unlink(argv[2]); 
    
    // 2. İki pipe'ı oluştur: (argv[1] -> İstemciden İsteği Okuma, argv[2] -> İstemciye Cevap Yazma)
    // Client Request Pipe (Sunucu okuma, İstemci yazma)
    if (mkfifo(argv[1], 0666) < 0) {
        perror("ISTEK PIPE (REQ) OLUSTURMA HATASI"); 
        exit(1);
    }
    printf("ISTEK PIPE (%s) OLUSTURULDU.\n", argv[1]);
    
    // Server Response Pipe (Sunucu yazma, İstemci okuma)
    if (mkfifo(argv[2], 0666) < 0) { 
        perror("CEVAP PIPE (RES) OLUSTURMA HATASI"); 
        // Eğer ikincisi başarısız olursa, ilkini silmek iyi bir uygulamadır
        unlink(argv[1]); 
        exit(1);
    } 
    printf("CEVAP PIPE (%s) OLUSTURULDU.\n", argv[2]);

    // Orijinal kodda izin ayarı vardı, bu adımı isteğe bağlı bırakıyorum:
    // chmod(argv[1], 0666); 
    // chmod(argv[2], 0666); 
    
    // --- BÖLÜM A: İSTEK ALMA VE CEVAP GÖNDERME ---
    
    // 3. İstemcinin istek göndermesi için İSTEK PIPE'ını (argv[1]) okuma modunda aç
    // Bu open() çağrısı, bir İstemci yazma modunda açana kadar BLOKE olur.
    printf("\nISTEK PIPE'I OKUMA ICIN ACILIYOR (%s) ve Istemci bekleniyor...\n", argv[1]);
    if ((fd_req = open(argv[1], O_RDONLY)) < 0) { 
        perror("ISTEK PIPE ACMA HATASI (O_RDONLY)"); 
        exit(1);
    }
    printf("ISTEK PIPE'I BASARIYLA ACILDI. Istemci baglandi.\n"); 
    
    // 4. İstemciden gelen ilk mesajı (istek) oku
    printf("ISTEMCI ISTEGI OKUNUYOR...\n"); 
    n_read = read(fd_req, buf, sizeof(buf) - 1);
    
    if (n_read > 0) {
        buf[n_read] = '\0'; 
        printf("ALINAN ISTEK MESAJI: %s\n", buf);
        
        // 5. Cevap göndermek için CEVAP PIPE'ını (argv[2]) yazma modunda aç
        // Client'ın bu pipe'ı okuma modunda açması BLOKAJI çözer.
        if ((fd_res = open(argv[2], O_WRONLY)) < 0) { 
            perror("CEVAP PIPE ACMA HATASI (O_WRONLY)"); 
            close(fd_req);
            exit(1);
        } 
        printf("CEVAP PIPE'I BASARIYLA ACILDI. Cevap gonderiliyor...\n");
        
        // 6. Cevabı İstemciye yaz
        write(fd_res, REPLY_MESSAGE, strlen(REPLY_MESSAGE) + 1); 
        printf("CEVAP MESAJI (%s) Istemciye gonderildi.\n", REPLY_MESSAGE);
        
        // 7. Cevap pipe'ını kapat
        close(fd_res); 
    }
    
    // 8. İstemcinin "bye" mesajını bekleme adımı
    // İlk okuma işlemi tamamlandı, pipe'ın okuma ucunu kapatıp tekrar açmamız gerekir.
    // Ancak FIFO'larda tek bir okuma süreci tarafından kapatılan uç, EOF sinyali gönderir.
    // Bu akış için, ilk mesajı aldıktan sonra İstemci tarafından pipe'ın kapatılıp
    // tekrar açılmasını bekleyen bir model uygulayacağız.
    
    // İlk istek pipe'ının okuma ucunu kapat (EOF sinyalini aldıktan sonra)
    close(fd_req);
    
    // Client'ın "bye" mesajını göndermesi için İSTEK PIPE'ını (argv[1]) tekrar aç
    // Bu open() çağrısı, client'ın 'bye' mesajını yollamak için bağlanmasını tekrar bekler.
    printf("\nCLIENT'IN SONLANMA ISTEGI ('bye') BEKLENIYOR...\n");
    if ((fd_req = open(argv[1], O_RDONLY)) < 0) {
        perror("ISTEK PIPE ACMA HATASI (BYE BEKLERKEN)"); 
        exit(1);
    }
    
    // "bye" mesajını oku
    n_read = read(fd_req, buf, sizeof(buf) - 1);
    if (n_read > 0) {
        buf[n_read] = '\0'; 
        if (strcmp(buf, BYE_MESSAGE) == 0) {
            printf("CLIENT'TAN 'bye' MESAJI ALINDI. Pipelar siliniyor.\n");
        } else {
            printf("CLIENT'TAN BEKLENMEYEN MESAJ ALINDI: %s\n", buf);
        }
    }

    // 9. Pipe'ları sil [cite: 74]
    unlink(argv[1]); 
    unlink(argv[2]); 
    
    printf("SERVER SONLANDIRILDI.\n");
    exit(0);
}
