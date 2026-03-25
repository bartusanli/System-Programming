#include <stdio.h>
#include <stdlib.h> // exit() için
#include <unistd.h> // fork(), pipe(), close(), read(), write() için
#include <sys/wait.h> // wait() için

// Parent-Child arası IPC (Manual write/read)
int main(void) {
    int fd[2];  /* Adsız pipe için tanımlayıcı dizisi: fd[0] okuma, fd[1] yazma */
    int pid1, pid2; /* Süreç tanımlayıcıları */

    /* Pipe, fork()'tan ÖNCE oluşturulmalıdır. */
    if (pipe(fd) < 0) {
        perror("PIPE OLUSTURMA HATASI");
        exit(1);
    }
    
    // --- BİRİNCİ ÇOCUK SÜREÇ (OKUYUCU) OLUŞTURULUYOR ---
    pid1 = fork();
    if (pid1 < 0) {
        perror("ILK FORK HATASI");
        exit(1);
    }

    if (pid1 == 0) { 
        // 1. Child: Reader (Okuyucu)
        char buf[100];
        ssize_t nbytes; // read() fonksiyonu ssize_t tipinde değer döndürür

        // Okuyucu, pipe'ın yazma ucunu KULLANMAYACAKTIR.
        close(fd[1]); 
        
        printf("Okuyucu (%d): Pipe'tan okumaya basliyorum...\n", getpid());
        
        // Pipe'tan okuma işlemi. read(), okunan bayt sayısını döndürür. 0 döndüğünde EOF'tur.
        while ((nbytes = read(fd[0], buf, sizeof(buf) - 1)) > 0) {
            // Okunan veriyi ekrana (Standart Çıkış = 1) yazdır.
            write(1, buf, nbytes);
        }
        
        // Okuma işi bitti, pipe'ın okuma ucunu kapat.
        close(fd[0]);
        
        printf("Okuyucu (%d): Okuma islemi tamamlandi, cikiyorum.\n", getpid());
        exit(0);
    } 
    
    // --- İKİNCİ ÇOCUK SÜREÇ (YAZICI) OLUŞTURULUYOR ---
    // Parent süreç devam ediyor ve ikinci çocuğu oluşturuyor
    pid2 = fork();
    if (pid2 < 0) {
        perror("IKINCI FORK HATASI");
        exit(1);
    }

    if (pid2 == 0) { 
        // 2. Child: Writer (Yazıcı)
        
        // Yazıcı, pipe'ın okuma ucunu KULLANMAYACAKTIR.
        close(fd[0]); 

        printf("Yazici (%d): Mesajlari pipe'a yaziyorum...\n", getpid());
        // Yazma işlemleri: write(dosya_tanimlayicisi, buffer, boyut)
        
        // Verilen kelimeleri pipe'ın yazma ucuna yazma:
        write(fd[1], "this\n", 5);
        write(fd[1], "is\n", 3);
        write(fd[1], "a\n", 2);
        write(fd[1], "message\n", 8);
        write(fd[1], "from\n", 5);
        write(fd[1], "sending\n", 8);
        write(fd[1], "process\n", 8);

        // Yazma işi bitti, pipe'ın yazma ucunu kapat. 
        // Bu, Okuyucu sürece verinin bittiğini (EOF) bildirir.
        close(fd[1]);
        
        printf("Yazici (%d): Yazma islemi tamamlandi, cikiyorum.\n", getpid());
        exit(0);
    } 
    
    // --- ANA SÜREÇ (PARENT) DEVAM EDİYOR ---
    // Parent, pipe'ın kendi uçlarını kapatır
    close(fd[0]);
    close(fd[1]);

    // Her iki çocuğun da sonlanmasını bekler
    waitpid(pid1, NULL, 0); // Okuyucunun bitmesini bekle
    waitpid(pid2, NULL, 0); // Yazıcının bitmesini bekle
    
    printf("Ana Süreç Tamamlandı.\n");
    return 0;
}
