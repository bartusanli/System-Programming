#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_MSG_SIZE 100
#define TEST_MESSAGE "Merhaba, bu bir yankilama (echo) mesajidir."

// Pipe Tanımlayıcıları:
// fd_pc: Parent'tan Child'a (pc) iletişim için pipe
// fd_cp: Child'dan Parent'a (cp) iletişim için pipe
int fd_pc[2]; // fd_pc[0]: Okuma ucu (Child kullanır), fd_pc[1]: Yazma ucu (Parent kullanır)
int fd_cp[2]; // fd_cp[0]: Okuma ucu (Parent kullanır), fd_cp[1]: Yazma ucu (Child kullanır)

void parent_process(void) {
    char read_buf[MAX_MSG_SIZE];
    ssize_t nbytes;

    printf("PARENT: Basliyor (PID: %d)\n", getpid());
    
    // Parent, Child'dan Okuma (fd_cp[0]) ve Child'a Yazma (fd_pc[1]) uçlarını kullanacak.
    // Kullanılmayan uçları kapat.
    close(fd_pc[0]); // Parent, Child'dan okumayı kullanmaz
    close(fd_cp[1]); // Parent, Child'a yazmayı kullanmaz

    printf("PARENT: Mesaji Child'a gonderiyor: \"%s\"\n", TEST_MESSAGE);
    
    // Parent, Pipe 1'e yazar
    write(fd_pc[1], TEST_MESSAGE, strlen(TEST_MESSAGE) + 1); // +1 null karakter için

    // Parent, Pipe 1'in yazma ucunu kapatır (Child'a mesajın bittiğini bildirir)
    close(fd_pc[1]);

    // Parent, Pipe 2'den Child'ın yolladığı yankı mesajını okur
    printf("PARENT: Child'dan yankilanan mesaji bekliyor...\n");
    nbytes = read(fd_cp[0], read_buf, MAX_MSG_SIZE);

    if (nbytes > 0) {
        // Okunan veriyi ekrana yazdır
        printf("PARENT: %zd byte alindi. YANKI MESAJI: \"%s\"\n", nbytes, read_buf);
    } else if (nbytes == 0) {
        printf("PARENT: Child pipe'i kapatti, veri gelmedi.\n");
    } else {
        perror("PARENT READ HATASI");
    }

    // Parent, Pipe 2'nin okuma ucunu kapatır
    close(fd_cp[0]);
    
    // Child'ın bitmesini bekle
    wait(NULL);
    printf("PARENT: Tamamlandi.\n");
}

void child_process(void) {
    char read_buf[MAX_MSG_SIZE];
    ssize_t nbytes;

    printf("CHILD: Basliyor (PID: %d)\n", getpid());

    // Child, Parent'tan Okuma (fd_pc[0]) ve Parent'a Yazma (fd_cp[1]) uçlarını kullanacak.
    // Kullanılmayan uçları kapat.
    close(fd_pc[1]); // Child, Parent'a yazmayı kullanmaz
    close(fd_cp[0]); // Child, Parent'tan okumayı kullanmaz

    // Child, Pipe 1'den Parent'ın mesajını okur
    printf("CHILD: Parent'tan mesaji bekliyor...\n");
    nbytes = read(fd_pc[0], read_buf, MAX_MSG_SIZE);
    
    if (nbytes > 0) {
        printf("CHILD: %zd byte alindi. Mesaj: \"%s\"\n", nbytes, read_buf);
        
        // Child, mesajı Parent'a geri yankılar (Pipe 2'ye yazar)
        printf("CHILD: Mesaji Parent'a geri yankiliyor.\n");
        write(fd_cp[1], read_buf, nbytes);
    } else if (nbytes < 0) {
        perror("CHILD READ HATASI");
    }

    // Child, Pipe 1'in okuma ucunu ve Pipe 2'nin yazma ucunu kapatır
    close(fd_pc[0]);
    close(fd_cp[1]);
    
    printf("CHILD: Tamamlandi.\n");
    exit(0);
}


int main(void) {

    // 1. Parent -> Child (P->C) pipe'ını oluştur
    if (pipe(fd_pc) < 0) {
        perror("P->C PIPE HATASI");
        return 1;
    }

    // 2. Child -> Parent (C->P) pipe'ını oluştur
    if (pipe(fd_cp) < 0) {
        perror("C->P PIPE HATASI");
        return 1;
    }

    // 3. Child sürecini oluştur
    pid_t pid = fork();

    if (pid < 0) {
        perror("FORK HATASI");
        return 1;
    }

    if (pid == 0) {
        // CHILD Süreci
        child_process();
    } else {
        // PARENT Süreci
        parent_process();
    }

    return 0;
}
