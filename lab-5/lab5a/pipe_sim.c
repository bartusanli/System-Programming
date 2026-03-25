#include <stdio.h>
#include <stdlib.h> // exit() için [cite: 19]
#include <unistd.h> // fork(), pipe(), dup2(), execlp() için [cite: 19]
#include <sys/wait.h> // wait() için

// Parent-Child arası IPC (getent group | grep kullanici_adi)
int main(void) {
    int fd[2];  /* Adsız pipe için iki tanımlayıcı dizisi: fd[0] okuma, fd[1] yazma [cite: 6, 19] */
    int pid1, pid2; /* Süreç tanımlayıcıları [cite: 20] */
    const char *kullanici_adi = "bartu_sanli"; // <<<<< BURAYI DEĞİŞTİRİN

    /* Pipe, fork()'tan ÖNCE oluşturulmalıdır ki, child süreçler kopyasına sahip olsun [cite: 21] */
    if (pipe(fd) < 0) {
        perror("PIPE OLUSTURMA HATASI");
        exit(1);
    }
    
    // --- BİRİNCİ ÇOCUK SÜREÇ (GREP) OLUŞTURULUYOR (Pipe'tan okur) ---
    pid1 = fork();
    if (pid1 < 0) {
        perror("ILK FORK HATASI");
        exit(1);
    }

    if (pid1 == 0) { 
        // 1. Child: grep
        
        // Pipe'ın okuma ucunu (fd[0]) standart girişe (0) yönlendir.
        // Böylece grep komutu pipe'tan okuma yapacaktır. [cite: 23]
        dup2(fd[0], 0); 

        // Bu süreç pipe'ın yazma ucunu kullanmayacak, bu yüzden kapatılır. [cite: 24]
        close(fd[1]);
        
        // Bu süreç pipe'ın okuma ucunu da kullanmayacak (çünkü dup2 ile 0'a yönlendirdik), kapatılır.
        close(fd[0]); 

        // "grep kullanıcı_adı" komutu çalıştırılır. [cite: 25]
        // execlp, başarılı olursa bu süreci grep ile değiştirir.
        execlp("grep", "grep", kullanici_adi, (char *)0);
        
        // execlp başarısız olursa buradan devam eder
        perror("GREP HATA");
        exit(1);
    } 
    
    // --- İKİNCİ ÇOCUK SÜREÇ (GETENT) OLUŞTURULUYOR (Pipe'a yazar) ---
    // Parent süreç devam ediyor ve ikinci çocuğu oluşturuyor [cite: 26]
    pid2 = fork();
    if (pid2 < 0) {
        perror("IKINCI FORK HATASI");
        exit(1);
    }

    if (pid2 == 0) { 
        // 2. Child: getent
        
        // Pipe'ın yazma ucunu (fd[1]) standart çıkışa (1) yönlendir.
        // Böylece getent komutunun çıktısı pipe'a yazılacaktır. [cite: 27]
        dup2(fd[1], 1); 

        // Bu süreç pipe'ın okuma ucunu kullanmayacak, bu yüzden kapatılır. [cite: 28]
        close(fd[0]);
        
        // Bu süreç pipe'ın yazma ucunu da kullanmayacak (çünkü dup2 ile 1'e yönlendirdik), kapatılır.
        close(fd[1]);

        // "getent group" komutu çalıştırılır. [cite: 29]
        execlp("getent", "getent", "group", (char *)0);
        
        // execlp başarısız olursa buradan devam eder
        perror("GETENT HATA");
        exit(1);
    } 
    
    // --- ANA SÜREÇ (PARENT) DEVAM EDİYOR ---
    // Parent, pipe'ın kendi uçlarını kapatır [cite: 30]
    close(fd[0]);
    close(fd[1]);

    // Her iki çocuğun da sonlanmasını bekler [cite: 30]
    waitpid(pid1, NULL, 0); // grep'in bitmesini bekle
    waitpid(pid2, NULL, 0); // getent'in bitmesini bekle
    
    printf("Ana Süreç Tamamlandı.\n");
    return 0;
}
