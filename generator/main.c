#include <stdio.h>
#include <stdlib.h>

typedef struct Next {
    char character;
    double probability;
} Next;

const int arraySize = 128;
int ngramArray[128];
int ngram = 2;

void writeInFile();
void createNgramme();
void getData();

int main() {
//    createNgramme();
    getData();

    return 0;
}

void getData() {
    FILE *fptr;

// Open file in read mode
    fptr = fopen("../ngramme.csv", "r");

    if (fptr == NULL) printf("File not found");

    char myString[100];
    Next data[256];

    while(fgets(myString, 100, fptr)) {
        printf("%s", myString);

        char character = myString[1];
        double probability = 0.56; //todo: string to double
        
        data[myString[0]].character = character;
        data[myString[0]].probability = probability;
    }

// Print the file content
    printf("%s", myString);

// Close the file
    fclose(fptr);

//    return data;
}








// Erzeugen von Testdaten

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
            ngramArray[c] += 1;
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
        fprintf(fptr, "%c,%d \n", i, ngramArray[i]);
    }

    // Close the file
    fclose(fptr);
}

