#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_FILES 10
#define MAX_NAME 32
#define MAX_SIZE 1024

typedef struct {
  char name[MAX_NAME];
  char data[MAX_SIZE];
  size_t size;
  bool isFree;  // flag: true -> dosya silinmiş / free
} FileEntry;

// Basit dosya sistemi simülasyonu
FileEntry fs[MAX_FILES];

// Dosya oluştur
int create_file(const char *name, const char *content) {
  for (int i = 0; i < MAX_FILES; i++) {
    if (fs[i].isFree) {  // boş slot bul
      strncpy(fs[i].name, name, MAX_NAME-1);
      fs[i].name[MAX_NAME-1] = '\0';
      strncpy(fs[i].data, content, MAX_SIZE-1);
      fs[i].data[MAX_SIZE-1] = '\0';
      fs[i].size = strlen(content);
      fs[i].isFree = false;
      return i; // dosya indeksi
    }
  }
  return -1; // alan yok
}

// Dosya sil
bool delete_file(const char *name) {
  for (int i = 0; i < MAX_FILES; i++) {
    if (!fs[i].isFree && strcmp(fs[i].name, name) == 0) {
      fs[i].isFree = true;   // sadece flag değişti
      printf("[FS/SUCCESS] FILE DELETED: %s\n", name);
      return true;
    }
  }
  return false;
}

// Dosya yazdır
void print_fs() {
  printf("Dosya tablosu:\n");
  for (int i = 0; i < MAX_FILES; i++) {
    if (!fs[i].isFree) {
      printf("  %s (%zu bytes)\n", fs[i].name, fs[i].size);
    } else {
      printf("  [FREE SLOT]\n");
    }
  }
}

// Belirli bir dosyayı yazdır
void print_file(const char *name) {
  for (int i = 0; i < MAX_FILES; i++) {
    if (!fs[i].isFree && strcmp(fs[i].name, name) == 0) {
      printf("Dosya: %s\nIçerik: %s\n", fs[i].name, fs[i].data);
      return;
    }
  }
  printf("FILE NOT FOUND: %s\n", name);
}

/* Yeni: fs içindeki tüm raw byte'ları sırayla yazdırır */
void dump_all_bytes(void) {
  unsigned char *p = (unsigned char *)fs;
  size_t total = sizeof(fs);
  for (size_t i = 0; i < total; i++) {
    unsigned char b = p[i];
    char c = (b >= 32 && b <= 126) ? (char)b : '.';
    printf("%06zu: %02x '%c'\n", i, b, c);
  }
}

/* Alternatif: belli bir aralığı yazdırmak istersen bu fonksiyonu kullan */
void dump_range(size_t start, size_t length) {
  unsigned char *p = (unsigned char *)fs;
  size_t total = sizeof(fs);
  if (start >= total) return;
  if (start + length > total) length = total - start;
  for (size_t i = 0; i < length; i++) {
    unsigned char b = p[start + i];
    char c = (b >= 32 && b <= 126) ? (char)b : '.';
    printf("%06zu: %02x '%c'\n", start + i, b, c);
  }
}

int main(int argc, char *argv[]) {
  // Başlangıçta tüm slotlar boş
  for (int i = 0; i < MAX_FILES; i++) fs[i].isFree = true;

  create_file("file1.txt", "Hello world");
  create_file("file2.txt", "Another file");

  printf("\n--- RAW BYTES DUMP STARTS ---\n");
  dump_all_bytes();
  printf("------ RAW BYTES DUMP ENDS  ------\n");


  print_file("file1.txt");
  print_file("file2.txt");
  print_fs();

  printf("\nFile deletion: file1.txt\n\n");
  delete_file("file1.txt");
  print_fs();
  print_file("file1.txt");
  print_file("file2.txt");

  printf("\n--- RAW BYTES DUMP STARTS ---\n");
  dump_all_bytes();
  printf("------ DUMP ENDS  ------\n");

  return 0;
}
