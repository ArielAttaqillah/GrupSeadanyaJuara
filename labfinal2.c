#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> 
#include <time.h> 

typedef struct {
    char nama[50];
    float harga;
    int jumlah;
} Barang;

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

int compareIgnoreCase(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2)) {
            return 0;
        }
        str1++;
        str2++;
    }
    return *str1 == '\0' && *str2 == '\0';
}

int compareHarga(const void *a, const void *b) {
    float hargaA = ((Barang*)a)->harga;
    float hargaB = ((Barang*)b)->harga;
    return (hargaA > hargaB) - (hargaA < hargaB);  
}

int compareBarangDibeli(const void *a, const void *b) {
    float hargaA = ((Barang*)a)->harga;
    float hargaB = ((Barang*)b)->harga;
    return (hargaA > hargaB) - (hargaA < hargaB);  
}

void printStruk(const char *filename, Barang *items, int jumlahBarang, float totalHarga, float diskon, float kembalian) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

  
    fprintf(file, "===== D'Store 25 =====\n");
    fprintf(file, "--------------------\n");

   
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(file, "Tanggal: %02d-%02d-%d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(file, "--------------------\n");

    
    for (int i = 0; i < jumlahBarang; i++) {
        char hargaFormatted[50];
        formatHarga(hargaFormatted, items[i].harga * items[i].jumlah);
        fprintf(file, "%-20s  %-10d  %s\n", items[i].nama, items[i].jumlah, hargaFormatted); // Perbaikan di sini
    }


    if (diskon > 0) {
        char diskonFormatted[50];
        formatHarga(diskonFormatted, diskon);
        fprintf(file, "Diskon: %s\n", diskonFormatted);
    }


    char totalFormatted[50];
    formatHarga(totalFormatted, totalHarga);
    fprintf(file, "Total: %s\n", totalFormatted);


    if (kembalian > 0) {
        char kembalianFormatted[50];
        formatHarga(kembalianFormatted, kembalian);
        fprintf(file, "Kembalian: %s\n", kembalianFormatted);
    }

    fprintf(file, "Terima kasih telah berbelanja di D'Store 25\n");

    fclose(file);
}

int main() {
    Barang toko[10] = {
        {"Shampoo", 25000.0, 10},
        {"Sabun", 20000.0, 15},
        {"Pasta Gigi", 15000.0, 20},
        {"Roti Tawar", 12000.0, 50},
        {"Teh Botol", 8000.0, 30},
        {"Mie Instan", 4000.0, 40},
        {"Kecap", 15000.0, 25},
        {"Minyak Goreng", 35000.0, 10},
        {"Beras 5 Kg", 75000.0, 20},
        {"Gula Pasir", 15000.0, 10}
    };

    Barang items[50];
    int jumlahBarang = 0;
    char pilihan;
    float totalHarga = 0.0;
    float diskon = 0.0;
    float kembalian = 0.0;


    printf("=== Selamat Datang di Toko D'Store 25 ===\n");
    printf("Kami menyediakan beragam jenis barang.\n");
    

    qsort(toko, 10, sizeof(Barang), compareHarga);

    printf("=== Daftar Barang Toko ===\n");
    printf("-------------------------------------------------------------\n");
    printf("| No  | %-20s | %-10s |\n", "Nama Barang", "Harga (Rp)");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < 10; i++) {
        char hargaFormatted[50];
        formatHarga(hargaFormatted, toko[i].harga);
        printf("| %-3d | %-20s | %-10s |\n", i + 1, toko[i].nama, hargaFormatted);
    }
    printf("-------------------------------------------------------------\n");

    do {
        char barangDipilih[50];
        int ditemukan = 0;

        printf("\nMasukkan barang yang ingin Anda beli: ");
        scanf(" %[^\n]s", barangDipilih);

      
        while ((getchar()) != '\n');

        for (int i = 0; i < 10; i++) {
            if (compareIgnoreCase(barangDipilih, toko[i].nama)) {
                strcpy(items[jumlahBarang].nama, toko[i].nama);
                items[jumlahBarang].harga = toko[i].harga;

                printf("Harga barang: ");
                char hargaFormatted[50];
                formatHarga(hargaFormatted, items[jumlahBarang].harga);
                printf("%s\n", hargaFormatted);

                printf("Masukkan jumlah barang: ");
                scanf("%d", &items[jumlahBarang].jumlah);

                jumlahBarang++;
                ditemukan = 1;
                break;
            }
        }

        if (!ditemukan) {
            printf("Barang tidak ditemukan. Silakan pilih barang yang ada dalam daftar.\n");
        } else {
            totalHarga = 0.0;
            for (int i = 0; i < jumlahBarang; i++) {
                totalHarga += items[i].harga * items[i].jumlah;
            }

            char totalFormatted[50];
            formatHarga(totalFormatted, totalHarga);
            printf("Total Harga Saat Ini: %s\n", totalFormatted);
        }

        printf("\nApakah Anda ingin menambahkan barang lagi? (y/n): ");
        scanf(" %c", &pilihan);
        printf("\n");

        
        while ((getchar()) != '\n');

    } while (pilihan == 'y' || pilihan == 'Y');
    

    char kodeVoucher[50];
    printf("Apakah Anda memiliki kode voucher? (y/n): ");
    scanf(" %c", &pilihan);
    if (pilihan == 'y' || pilihan == 'Y') {
        printf("Masukkan kode voucher: ");
        scanf(" %[^\n]s", kodeVoucher);
        if (compareIgnoreCase(kodeVoucher, "Pr0m0aKhiRBuLan")) {
            diskon = totalHarga * 0.10; 
            totalHarga -= diskon;
            printf("Diskon 10%% diterapkan. Total harga setelah diskon: ");
            char totalFormatted[50];
            formatHarga(totalFormatted, totalHarga);
            printf("%s\n", totalFormatted);
        } else {
            printf("Kode voucher tidak valid.\n");
        }
    }

    printf("--------------------\n");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%02d-%02d-%d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("--------------------\n");


    qsort(items, jumlahBarang, sizeof(Barang), compareBarangDibeli);

    printf("\n=== Daftar Barang Yang Dibeli ===\n");
    printf("-------------------------------------------------------------\n");
    printf("| %-20s | %-10s | %-8s | %-10s |\n", "Nama Barang", "Harga", "Jumlah", "Subtotal");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < jumlahBarang; i++) {
        char hargaFormatted[50], subtotalFormatted[50];
        formatHarga(hargaFormatted, items[i].harga);
        formatHarga(subtotalFormatted, items[i].harga * items[i].jumlah);
        printf("| %-20s | %-10s | %-8d | %-10s |\n",
               items[i].nama, hargaFormatted, items[i].jumlah, subtotalFormatted);
    }
    printf("-------------------------------------------------------------\n");

    char totalFormatted[50];
    formatHarga(totalFormatted, totalHarga);
    printf("Total Harga Akhir: %s\n", totalFormatted);

    
pilih_metode:
    printf("\n=== Metode Pembayaran ===\n");
    printf("1. Tunai\n");
    printf("2. Kartu Kredit\n");
    printf("3. Transfer Bank\n");

    int metode;
    printf("Pilih metode pembayaran (1-3): ");
    scanf("%d", &metode);

    switch (metode) {
        case 1: {
            float jumlahDibayar;
            printf("Anda memilih pembayaran secara tunai.\n");
            printf("Masukkan jumlah uang yang dibayar: Rp ");
            scanf("%f", &jumlahDibayar);

            if (jumlahDibayar >= totalHarga) {
                kembalian = jumlahDibayar - totalHarga;
                char kembalianFormatted[50];
                formatHarga(kembalianFormatted, kembalian);
                sleep(2);
                printf("Pembayaran berhasil.\n");
                printf("Kembalian Anda: %s\n", kembalianFormatted);
                printf("\n");
            } else {
                printf("Maaf, saldo Anda tidak mencukupi. Transaksi gagal.\n");
                char pilihMetodeLain;
                printf("Apakah Anda ingin memilih metode pembayaran lain? (y/n): ");
                scanf(" %c", &pilihMetodeLain);

                
                while ((getchar()) != '\n');

                if (pilihMetodeLain == 'y' || pilihMetodeLain == 'Y') {
                    goto pilih_metode; 
                } else {
                    return 1; 
                }
            }
            break;
        }
        case 2:
            printf("Anda memilih pembayaran menggunakan kartu kredit.\n");
            sleep(2); 
            printf("Silahkan melanjutkan pembayaran di Cashier kami.\n");
            printf("\n");
            break;
        case 3: {
            printf("Anda memilih pembayaran melalui transfer bank.\n");
            printf("\nSilakan pilih bank berikut:\n");
            printf("1. BCA - 8630530599\n");
            printf("2. BRI - 058301000483307\n");
            printf("3. Mandiri - 1410014544803\n");
            printf("4. BNI - 817215953\n");

            int bankPilihan;
            printf("Masukkan nomor pilihan bank (1-4): ");
            scanf("%d", &bankPilihan);

            char totalFormatted[50];
            formatHarga(totalFormatted, totalHarga);

            switch (bankPilihan) {
                case 1:
                    sleep(2); 
                    printf("\nNama Bank: BCA \n");
                    printf ("Atas Nama: PT. D'Store 25\n");
                    printf("Nomor Virtual Account: 8630530599\n");
                    printf("Total Nominal: %s\n", totalFormatted);
                    break;
                case 2:
                    sleep(2); 
                    printf("\nNama Bank: BRI\n");
                    printf("Atas Nama: PT. D'Store 25\n");
                    printf("Nomor Virtual Account: 058301000483307\n");
                    printf("Total Nominal: %s\n", totalFormatted);
                    break;
                case 3:
                    sleep(2); 
                    printf("\nNama Bank: Mandiri\n");
                    printf("Atas Nama: PT. D'Store 25\n");
                    printf("Nomor Virtual Account: 1410014544803\n");
                    printf("Total Nominal: %s\n", totalFormatted);
                    break;
                case 4:
                    sleep(2);
                    printf("\nNama Bank: BNI\n");
                    printf("Atas Nama: PT. D'Store 25\n");
                    printf("Nomor Virtual Account: 817215953\n");
                    printf("Total Nominal: %s\n", totalFormatted);
                    break;
                default:
                    printf("Pilihan tidak valid. Transaksi dibatalkan.\n");
                    return 1;
            }

            sleep(5); 
            printf("\nTransaksi Berhasil\n");
            printf("\n");            
            break;
        }
        default:
            printf("Pilihan tidak valid. Silakan ulangi transaksi.\n");
            return 1;
    }

   
    printStruk("strukfinal.txt", items, jumlahBarang, totalHarga, diskon, kembalian);

  
    printf("Terima kasih telah berbelanja di D'Store 25!\n");

    return 0;
}