# Lab3 - System Programming (Bölüm 1 ve 2)
# Bartu tarafından hazırlanan dosya kopyalama programı

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = copy_parallel

# Varsayılan hedef
all: $(TARGET)

# copy_parallel derleme kuralı
$(TARGET): copy_parallel.c
	$(CC) $(CFLAGS) -o $(TARGET) copy_parallel.c

# Programı çalıştırmak için basit komut
run:
	./$(TARGET) input.txt output1.txt output2.txt

# Temizleme işlemi
clean:
	rm -f $(TARGET) *.o
