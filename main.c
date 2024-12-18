#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_BOOKS 100

// Struct untuk menyimpan data buku
typedef struct {
    int id;
    char title[100];
    char author[100];
    int year;
} Book;

// Fungsi Prototipe
void addBook(Book *books, int *count, const char *filename);
void displayBooks(Book *books, int count);
void saveToFile(Book *books, int count, const char *filename);
void loadFromFile(Book *books, int *count, const char *filename);
void sortBooksByYear(Book *books, int count);
int searchByAuthor(Book *books, int count, const char *author);
int countBooksByYear(Book *books, int count, int year);

int countBooksByYearRecursive(Book *books, int count, int year) {
    if (count == 0)
        return 0;
    return (books[count - 1].year == year) + countBooksByYearRecursive(books, count - 1, year);
}

int main() {
    Book books[MAX_BOOKS];
    Book sortedBooks[MAX_BOOKS];
    int count = 0;
    int choice;
    const char *filename = "books.txt";

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("File '%s' tidak ditemukan. File akan dibuat.", filename);
        file = fopen(filename, "w");
        if (!file) {
            printf("Gagal membuat file '%s'. Keluar dari program.", filename);
            return 1;
        }
        fclose(file);
    } else {
        fclose(file);
        loadFromFile(books, &count, filename);
    }

    do {
    	system("cls");
        printf("\nSistem Manajemen Buku Perpustakaan\n");
        printf("1. Tambah Buku\n");
        printf("2. Tampilkan Semua Buku\n");
        printf("3. Urutkan Buku Berdasarkan Tahun\n");
        printf("4. Cari Buku Berdasarkan Penulis\n");
        printf("5. Hitung Buku Berdasarkan Tahun\n");
        printf("6. Keluar\n");
        printf("Pilihan: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBook(books, &count, filename);
                getch();
                break;
            case 2:
                displayBooks(books, count);
                getch();
                break;
			case 3:
			    memcpy(sortedBooks, books, count * sizeof(Book)); 
			    sortBooksByYear(sortedBooks, count);             
			    printf("Buku berhasil diurutkan.\n");
			    displayBooks(sortedBooks, count);                
			    getch();
			    break;
            case 4: {
                char author[100];
                printf("Masukkan nama penulis: ");
                getchar();
                fgets(author, sizeof(author), stdin);
                author[strcspn(author, "\n")] = 0;
                int index = searchByAuthor(books, count, author);
                if (index != -1) {
                    printf("Buku ditemukan:\n");
                    printf("\n+------+--------------------------------+--------------------------+------+\n");
    				printf("|  ID  |            Judul              |         Penulis          | Tahun |\n");
				    printf("+------+--------------------------------+--------------------------+------+\n");
				    printf("| %4d | %-30s | %-24s | %4d |\n", books[index].id, books[index].title, books[index].author, books[index].year);
				    printf("+------+--------------------------------+--------------------------+------+\n");
                } else {
                    printf("Buku dengan penulis '%s' tidak ditemukan.\n", author);
                }
                getch();
                break;
            }
            case 5: {
                int year;
                printf("Masukkan tahun: ");
                scanf("%d", &year);
                int countByYear = countBooksByYearRecursive(books, count, year);
                printf("Jumlah buku yang diterbitkan pada tahun %d: %d\n", year, countByYear);
                getch();
                break;
            }
            case 6:
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
                getch();
        }
    } while (choice != 6);

    return 0;
}

void addBook(Book *books, int *count, const char *filename) {
    if (*count >= MAX_BOOKS) {
        printf("Kapasitas maksimum tercapai. Tidak dapat menambah buku lagi.\n");
        return;
    }

    Book *newBook = &books[*count];
    printf("Masukkan ID Buku: ");
    scanf("%d", &newBook->id);
    getchar();
    printf("Masukkan Judul Buku: ");
    fgets(newBook->title, sizeof(newBook->title), stdin);
    newBook->title[strcspn(newBook->title, "\n")] = 0;
    printf("Masukkan Penulis Buku: ");
    fgets(newBook->author, sizeof(newBook->author), stdin);
    newBook->author[strcspn(newBook->author, "\n")] = 0;
    printf("Masukkan Tahun Terbit: ");
    scanf("%d", &newBook->year);

    (*count)++;
    saveToFile(books, *count, filename);
    printf("Buku berhasil ditambahkan dan disimpan ke file!\n");
}

void displayBooks(Book *books, int count) {
    if (count == 0) {
        printf("Tidak ada buku untuk ditampilkan.\n");
        return;
    }

    printf("\n+------+--------------------------------+--------------------------+------+\n");
    printf("|  ID  |            Judul              |         Penulis          | Tahun |\n");
    printf("+------+--------------------------------+--------------------------+------+\n");

    for (int i = 0; i < count; i++) {
        printf("| %4d | %-30s | %-24s | %4d |\n", books[i].id, books[i].title, books[i].author, books[i].year);
    }

    printf("+------+--------------------------------+--------------------------+------+\n");
}

void saveToFile(Book *books, int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Gagal membuka file untuk menyimpan data.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d|%s|%s|%d\n", books[i].id, books[i].title, books[i].author, books[i].year);
    }

    fclose(file);
    printf("Data berhasil disimpan ke file '%s'.\n", filename);
}

void loadFromFile(Book *books, int *count, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("File tidak ditemukan. Data tidak dimuat.\n");
        return;
    }

    *count = 0;
    while (fscanf(file, "%d|%99[^|]|%99[^|]|%d\n", &books[*count].id, books[*count].title, books[*count].author, &books[*count].year) == 4) {
        (*count)++;
    }

    fclose(file);
    printf("Data berhasil dimuat dari file '%s'.\n", filename);
}

void sortBooksByYear(Book *books, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (books[j].year > books[j + 1].year) {
                Book temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }
}

int searchByAuthor(Book *books, int count, const char *author) {
    for (int i = 0; i < count; i++) {
        if (strcmp(books[i].author, author) == 0) {
            return i;
        }
    }
    return -1;
}
