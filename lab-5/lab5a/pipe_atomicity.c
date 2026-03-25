#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MESSAGE_SIZE 50

// Bir çocuk süreç için mesaj yazma fonksiyonu
void writer_process(int write_fd, int id, const char *message) {
    char full_message[MESSAGE_SIZE];
    
    // Mesajı hazırlama
    snprintf(full_message, MESSAGE_SIZE, "Mesaj %d: %s (PID: %d)\n", id, message, getpid());
    size_t len = strlen(full_message);
    
    printf("Yazici %d basladi, mesaji gonderiyor... (Uzunluk: %zu)\n", id, len);
    
    // Mesajı pipe'a yazma
    ssize_t written_bytes = write(write_fd, full_message, len);
    
    if (written_bytes != (ssize_t)len) {
        perror("WRITE HATASI");
    } else {
        printf("Yazici %d: %zd byte basariyla yazildi.\n", id, written_bytes);
    }
    
    exit(0);
}

// Okuyucu süreç
void reader_process(int read_fd) {
    char buf[200];
    ssize_t nbytes;
    
    printf("Okuyucu basladi, pipe'tan okumayi bekliyor... (PID: %d)\n", getpid());
    
    // Pipe'tan veri okuma döngüsü
    // read(), okuyacak veri kalmadığında ve tüm yazma uçları kapandığında 0 döndürür.
    while ((nbytes = read(read_fd, buf, sizeof(buf) - 1)) > 0) {
        // Okunan veriyi null sonlandır ve ekrana yazdır.
        buf[nbytes] = '\0';
        printf("Okuyucu: %zd byte alindi. Mesaj:\n%s", nbytes, buf);
    }
    
    if (nbytes == 0) {
        printf("Okuyucu: Tüm yazma uclari kapandi, okuma bitti.\n");
    } else {
        perror("READ HATASI");
    }
    
    exit(0);
}

int main(void) {
    int fd[2];  // fd[0] okuma, fd[1] yazma
    pid_t pid_w1, pid_w2, pid_r;
    
    if (pipe(fd) < 0) {
        perror("PIPE OLUSTURMA HATASI");
        exit(1);
    }
    
    // --- 1. ÇOCUK SÜREÇ (YAZICI 1) ---
    pid_w1 = fork();
    if (pid_w1 == 0) {
        close(fd[0]); // Yazıcı okuma ucunu kapatır
        writer_process(fd[1], 1, "Ilk mesajim.");
    } else if (pid_w1 < 0) {
        perror("Yazici 1 Fork Hatasi");
        exit(1);
    }

    // --- 2. ÇOCUK SÜREÇ (YAZICI 2) ---
    pid_w2 = fork();
    if (pid_w2 == 0) {
        close(fd[0]); // Yazıcı okuma ucunu kapatır
        writer_process(fd[1], 2, "Ikinci mesajim, daha uzun.");
    } else if (pid_w2 < 0) {
        perror("Yazici 2 Fork Hatasi");
        exit(1);
    }

    // --- 3. ÇOCUK SÜREÇ (OKUYUCU) ---
    pid_r = fork();
    if (pid_r == 0) {
        close(fd[1]); // Okuyucu yazma ucunu kapatır
        reader_process(fd[0]);
    } else if (pid_r < 0) {
        perror("Okuyucu Fork Hatasi");
        exit(1);
    }
    
    // --- ANA SÜREÇ (PARENT) ---
    // Parent, kendi pipe uçlarını kapatır (çünkü iletişime karışmayacak)
    close(fd[0]);
    close(fd[1]);
    
    // Tüm çocukların bitmesini bekle
    waitpid(pid_w1, NULL, 0);
    waitpid(pid_w2, NULL, 0);
    waitpid(pid_r, NULL, 0);

    printf("Ana Süreç: Tüm cocuklar sonlandi. Program sonu.\n");
    return 0;
}
