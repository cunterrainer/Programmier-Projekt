#include <stdio.h>
#include <stdlib.h>

const int arraySize = 128;
int ngrammArray[128];

void writeInFile();
void createNgramme();

void createNgramme() {
    FILE *datei;
    int c;

    // Datei öffnen
    datei = fopen("../generator/file.txt", "r");

    if (datei == NULL) {
        printf("Error: File not found");
    } else {
        // Zeichenweise lesen und ausgeben
        while ((c = fgetc(datei)) != EOF) {
            putchar(c);
            ngrammArray[c] += 1;
        }

        // Datei schließen
        fclose(datei);
        writeInFile();
    }

}

void writeInFile() {
    FILE *fptr;

    // Open a file in writing mode
    fptr = fopen("../ngramme.csv", "w");

    // Write some text to the file
    for (int i = 0; i < arraySize; i++) {
        fprintf(fptr, "%c,%d \n", i, ngrammArray[i]);
    }

    // Close the file
    fclose(fptr);
}

int main() {
    createNgramme();

    return 0;
}