#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Definisi struktur barang
typedef struct {
    char nama[50];
    float harga;
    int jumlah;
} Barang;

// Fungsi untuk memformat angka menjadi format mata uang Indonesia
void formatHarga(char *buffer, float harga) {
    int ribuan = (int) harga;
    int sisa = (int)((harga - ribuan) * 100);
    char ribuanFormatted[50];
    int i = 0, count = 0;

    sprintf(ribuanFormatted, "%d", ribuan);
    int len = strlen(ribuanFormatted);

    for (int j = len - 1; j >= 0; j--) {
        buffer[i++] = ribuanFormatted[j];
        count++;
        if (count % 3 == 0 && j > 0) {
            buffer[i++] = '.';
        }
    }

    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }

    sprintf(buffer + i, ",%02d", sisa);
    memmove(buffer + 3, buffer, strlen(buffer) + 1);
    buffer[0] = 'R';
    buffer[1] = 'p';
    buffer[2] = ' ';
}

void printStruk(const char *filename, Barang *items, int jumlahBarang, float totalHarga, float diskon, float kembalian) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Menampilkan header struk
    fprintf(file, "===== D'Store 25 =====\n");
    fprintf(file, "--------------------\n");

    // Menampilkan tanggal dan waktu
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(file, "Tanggal: %02d-%02d-%d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(file, "--------------------\n");

    // Menampilkan barang yang dibeli
    for (int i = 0; i < jumlahBarang; i++) {
        char hargaFormatted[50];
        formatHarga(hargaFormatted, items[i].harga * items[i].jumlah);
        fprintf(file, "%-20s  %-10d  %s\n", items[i].nama, items[i].jumlah, hargaFormatted);
    }

    // Menampilkan diskon jika ada
    if (diskon > 0) {
        char diskonFormatted[50];
        formatHarga(diskonFormatted, diskon);
        fprintf(file, "Diskon: %s\n", diskonFormatted);
    }

    // Menampilkan total harga
    char totalFormatted[50];
    formatHarga(totalFormatted, totalHarga);
    fprintf(file, "Total: %s\n", totalFormatted);

    // Menampilkan kembalian jika ada
    if (kembalian > 0) {
        char kembalianFormatted[50];
        formatHarga(kembalianFormatted, kembalian);
        fprintf(file, "Kembalian: %s\n", kembalianFormatted);
    }

    fprintf(file, "Terima kasih telah berbelanja di D'Store 25\n");

    fclose(file);
}

int main() {
    Barang items[50] = {
        {"Shampoo", 25000.0, 2},
        {"Sabun", 20000.0, 1},
        {"Pasta Gigi", 15000.0, 3}
    };
    int jumlahBarang = 3;
    float totalHarga = 0.0;
    float diskon = 0.0;
    float kembalian = 5000.0; // Contoh kembalian

    // Hitung total harga
    for (int i = 0; i < jumlahBarang; i++) {
        totalHarga += items[i].harga * items[i].jumlah;
    }

    // Misalkan ada diskon 10%
    diskon = totalHarga * 0.10;
    totalHarga -= diskon;

    // Cetak struk ke file
    printStruk("strukfinal.txt", items, jumlahBarang, totalHarga, diskon, kembalian);

    printf("Struk telah dicetak ke file 'struk.txt'.\n");
    return 0;
}