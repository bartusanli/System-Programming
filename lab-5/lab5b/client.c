/* A client using the FIFO pipes for two-way communication */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h> // exit() için
#include <unistd.h> // read(), write(), close() için
#include <string.h> // strlen() için

#define MAX_BUFFER 1024
#define REQUEST_MESSAGE "Hi Server, may I get a reply?"
#define BYE_MESSAGE "bye"

// Kullanım: ./client pipe_req pipe_res
int main(int argc, char *argv[]) {
    int fd_req, fd_res;
    ssize_t n_read;
    char buf[MAX_BUFFER];

    if (argc < 3) { 
        printf("Kullanim: %s <request_pipe_name> <response_pipe_name>\n", argv[0]);
        exit(1); 
    }
    
    // argv[1]: İstek Pipe Adı (Sunucu Okur, İstemci Yazar)
    // argv[2]: Cevap Pipe Adı (İstemci Okur, Sunucu Yazar)
    
    printf("CLIENT BASLADI... (PID: %d)\n", getpid());

    // --- BÖLÜM A: İSTEK GÖNDERME ---

    // 1. İstek göndermek için İSTEK PIPE'ını (argv[1]) yazma modunda aç
    // Bu open() çağrısı, Sunucu open() çağrısının BLOKAJINI çözer.
    printf("ISTEK PIPE'I YAZMA ICIN ACILIYOR (%s)...\n", argv[1]);
    if ((fd_req = open(argv[1], O_WRONLY)) < 0) { 
        perror("ISTEK PIPE ACMA HATASI (O_WRONLY)"); 
        exit(1);
    }
    printf("ISTEK PIPE'I ACILDI. Istek gonderiliyor.\n");
    
    // 2. İsteği Sunucuya yaz
    write(fd_req, REQUEST_MESSAGE, strlen(REQUEST_MESSAGE) + 1); 
    printf("ISTEK MESAJI ('%s') Sunucuya gonderildi.\n", REQUEST_MESSAGE);
    
    // 3. İstek pipe'ını kapat (İstek gönderimi tamamlandı)
    close(fd_req); 
    
    // --- BÖLÜM B: CEVAP ALMA ---
    
    // 4. Cevabı almak için CEVAP PIPE'ını (argv[2]) okuma modunda aç
    // Sunucu'nun yazma ucu açıldıktan sonra bu open() çağrısı BLOKE olmaz.
    printf("\nCEVAP PIPE'I OKUMA ICIN ACILIYOR (%s) ve Cevap bekleniyor...\n", argv[2]);
    if ((fd_res = open(argv[2], O_RDONLY)) < 0) { 
        perror("CEVAP PIPE ACMA HATASI (O_RDONLY)"); 
        exit(1);
    } 
    printf("CEVAP PIPE'I ACILDI. Cevap okunuyor.\n");
    
    // 5. Cevabı oku ve ekrana yazdır
    while ((n_read = read(fd_res, buf, sizeof(buf) - 1)) > 0) {
        buf[n_read] = '\0';
        printf("SUNUCUDAN ALINAN CEVAP: %s\n", buf);
    }
    
    // 6. Cevap pipe'ını kapat
    close(fd_res);

    // --- BÖLÜM C: SONLANMA ISTEGI (BYE) GÖNDERME ---
    
    // 7. Pipe'ları silmesi için Sunucuya 'bye' mesajını tekrar İSTEK PIPE'ından (argv[1]) gönder.
    // Pipe'ı tekrar yazma modunda aç.
    printf("\nSUNUCUYA KAPANMA ISTEGI ('%s') GONDERILIYOR...\n", BYE_MESSAGE);
    if ((fd_req = open(argv[1], O_WRONLY)) < 0) { 
        perror("ISTEK PIPE ACMA HATASI (BYE ICIN)"); 
        exit(1);
    }
    
    // 8. 'bye' mesajını yaz ve pipe'ı kapat
    write(fd_req, BYE_MESSAGE, strlen(BYE_MESSAGE) + 1);
    close(fd_req);
    
    printf("CLIENT SONLANDIRILDI.\n");
    return 0;
}
