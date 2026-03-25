#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BLOCK_SIZE 10   // okuma/yazma bloğu boyutu
#define N 10000         // busy waiting döngü sayısı (deney için değiştirilecek)

int main(int argc, char *argv[]) {
    int fd1, fd2, fd3;
    pid_t pid;
    char buffer[BLOCK_SIZE];
    ssize_t bytes_read;

    if (argc != 4) {
        fprintf(stderr, "Kullanım: %s dosya1 dosya2 dosya3\n", argv[0]);
        exit(1);
    }

    // Dosya1'i okumak için aç
    fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        perror("dosya1 açılamadı");
        exit(1);
    }

    // Child process oluştur
    pid = fork();

    if (pid < 0) {
        perror("fork hatası");
        exit(1);
    }

    // CHILD PROCESS
    else if (pid == 0) {
        fd3 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd3 < 0) {
            perror("dosya3 oluşturulamadı");
            exit(1);
        }

        // dosya1'den okuyup dosya3'e yaz
        while ((bytes_read = read(fd1, buffer, BLOCK_SIZE)) > 0) {
            for (int i = 0; i < N; i++); // Busy waiting
            write(fd3, buffer, bytes_read);
        }

        close(fd3);
        close(fd1);
        exit(0);
    }

    // PARENT PROCESS
    else {
        fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd2 < 0) {
            perror("dosya2 oluşturulamadı");
            exit(1);
        }

        // dosya1'den okuyup dosya2'ye yaz
        while ((bytes_read = read(fd1, buffer, BLOCK_SIZE)) > 0) {
            for (int i = 0; i < N; i++); // Busy waiting
            write(fd2, buffer, bytes_read);
        }

        close(fd2);
        close(fd1);
    }

    return 0;
}
