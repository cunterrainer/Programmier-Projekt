#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include<sys/time.h>
#include "main.h"

int main() {
//    createNgramme();
//    readFile();
    randomNgram();
//    readFileRandom();
//    readFileNew();
    readFileWithMalloc();
    initializeRandomNumSeed();

    // Settings
    textGenMode = 1;
    textLength = 99;

    // generate
    generateText();
    printf("%s", text);

    return 0;
}

/*todo:
 * - buchstaben vorgeben (ein Buchstabe done)
 * - Zeilenanfang markieren, z.b. durch festes Zeichen
 *      - generieren von Sätzen
 *      - zeilenumbrüche
*  - in array pointer speichern
 *      - array pointer -> pointer -> double wert / Null
 *      - für mehr als zwei Buchstaben Funktionen anpassen
 * - ...
 *
 * Done:
 * - header datei (done)
 * - testen (done)
 *
 * */


// neues Schema
//double* arrayPointer1[256];
//double* arrayPointer2[256];
//double* arrayPointer3[256];
//double value;
//
//value = 0.0233;
//arrayPointer2[0] = &value;
//arrayPointer1[0] = arrayPointer2;

//void readFileNew() {
//
//    FILE *fptr;
//    char line[13];
//    char doubleString[8];
////    double** pArray[256];
//    double** array = (double**)malloc(256 * sizeof(double*));
//    double* pArray2[256];
//    double pArray3[256];
//    int firstLetter = 0;
//    int secondLetter = 0;
//    int oldFirstLetter;
//    int oldSecondLetter;
//    int oldFirstLetterBool;
//    int oldSecondLetterBool;
//
//    // Open a file in writing mode
//    fptr = fopen("../ngrammeRandom.csv", "r");
//
//    while(fgets(line, 100, fptr)) {
//        oldFirstLetter = firstLetter;
//        firstLetter = line[0];
//        oldFirstLetterBool = (oldFirstLetter == firstLetter) ? 1 : 0;
//
//        oldSecondLetter = secondLetter;
//        secondLetter = line[1];
//        oldSecondLetterBool = (oldSecondLetter == secondLetter) ? 1 : 0;
//
//        int thirdLetter = line[2];
//
//        for (int i = 4; i < 12; i++) {
//            doubleString[i-4] = line[i];
//        }
//
//        double probability = 0.0;
//
//        sscanf(doubleString, "%lf", &probability);
//
////        dataArray[firstLetter][secondLetter] = probability;
//
//
//        if (oldFirstLetterBool == 0) {
////            double* pArray2[256];
//            array[firstLetter] = (double*)malloc(256 * sizeof(double));
//        }
//
//        if (oldSecondLetterBool == 0) {
//            double pArray3[256];
//            array
//            pArray3[thirdLetter] = probability;
//            array[firstLetter][secondLetter] = pArray3;
//            pArray[firstLetter] = pArray2;
//        }
//
//    }
//
//    // Close the file
//    fclose(fptr);
//
//}

int readFileWithMalloc() {
    FILE* fptr;
    // Open a file in writing mode
    fptr = fopen("../ngrammeRandom.csv", "r");
    char line[13];
    int isEOF = 0;

    // Define the size of the outer array
    int outer_size = 256;
    double*** array = (double***)malloc(outer_size * sizeof(double**));

    int middle_size = 256;
    int inner_size = 256;
    int trash = 256; // fgets ändert diesen wert auf 10, WARUM?!??!?! Wird er entfernt, wird die Variable darüber auf 10 gesetzt

    int oldFirstLetter = -1;
    int oldSecondLetter = -1;

    fgets(line, 100, fptr);
    int firstLetter = line[0];
    int secondLetter = line[1];
    int thirdLetter = line[2];

    char doubleString[8];
    for (int i = 4; i < 12; i++) {
        doubleString[i-4] = line[i];
    }

    double probability = 0.0;
    probability = atof(doubleString);

    for (int i = 0; i < outer_size; i++) {
        if (isEOF == 1) break;

        if (i != firstLetter) {
            array[i] = NULL;
            continue;
        }

        if (oldFirstLetter != firstLetter) {
            array[i] = (double**) malloc(middle_size * sizeof(double *));
            oldFirstLetter = firstLetter;
        }

        for (int j = 0; j < middle_size; j++) {
            if (isEOF == 1) break;

            if (j != secondLetter) {
                array[i][j] = NULL;
                continue;
            }

            if (oldSecondLetter != secondLetter) {
                array[i][j] = (double*) malloc(inner_size * sizeof(double));
                oldSecondLetter = secondLetter;
            }

            for (int k = 0; k < inner_size; k++) {
                if (k != thirdLetter) continue;

                array[i][j][k] = probability;  // Assign a simple value based on index
                if (!fgets(line, 100, fptr)) {
                    isEOF = 1;
                    break;
                }

                firstLetter = line[0];
                secondLetter = line[1];
                thirdLetter = line[2];

                for (int m = 4; m < 12; m++) {
                    doubleString[m-4] = line[m];
                }

                probability = atof(doubleString);
            }
        }
    }


    // Free and print the allocated memory to avoid memory leaks
    for (int i = 0; i < outer_size; i++) {
        if (array[i] == NULL) {
            printf("array[%d]: Null \n", i);
            continue;
        }
        for (int j = 0; j < middle_size; j++) {
            if (array[i][j] == NULL) {
                printf("array[%d][%d]: Null \n", i, j);
                continue;
            }
            for (int k = 0; k < inner_size; k++) {
                printf("Value at array[%d][%d][%d]: %.6f\n", i, j, k, array[i][j][k]); //todo: array[0][0[0]
            }
            free(array[i][j]);
        }
        free(array[i]);
    }
    free(array);

    // Close the file
    fclose(fptr);

    return 0;
}

void getNextLine(FILE* fptr) {
    fgets(lineTest, 100, fptr);
}

void getFirstLetterByUser(int letters[2]){
    printf("Start Buchstabe eingeben: ");
    char firstLetter = 'a';
    scanf("%c",&firstLetter);
    letters[0] = firstLetter;
}

void readFileRandom() {
    FILE *fptr;
    char line[13];
    char doubleString[8];

    // Open a file in writing mode
    fptr = fopen("../ngrammeRandom.csv", "r");

    while(fgets(line, 100, fptr)) {
        int firstLetter = line[0];
        int secondLetter = line[1];

        for (int i = 3; i < 12; i++) {
            doubleString[i-3] = line[i];
        }

        double probability = 0.0;
        sscanf(doubleString, "%lf", &probability);

        dataArray[firstLetter][secondLetter] = probability;
    }

    // Close the file
    fclose(fptr);
}

void generateText() {
    int letters[2];

//    getFirstLetterByHighestProbability(letters);
//    getFirstLettersByPercentageProbability(letters);
    getFirstLetterByUser(letters);

    text[0] = (char) letters[0];
//    text[1] = (char) letters[1];
//    letters[0] = letters[1];

    for (int i = 1; i < textLength; i++) {
        if (textGenMode == 0) {
            getNextLetterByHighestProbability(letters);
        } else if (textGenMode == 1) {
            getNextLetterByPercentageProbability(letters);
        }

        text[i] = (char) letters[1];
        letters[0] = letters[1];
    }

}

void getFirstLettersByPercentageProbability(int letters[2]) {
    double probabilities[65000][2] = {0};
    unsigned int counter = 0;
    for (int i = 65; i < 122; i++) {
        for (int j = 65; j < 122; j++) {
            if (dataArray[i][j] > requiredPercentage) {
                probabilities[counter][0] = i;
                probabilities[counter++][1] = j;
            }
        }
    }

    //random Buchstabe aus array zuweisen
    unsigned int randomNum = 257;
    randomNum = getRandomNum(counter);

    letters[0] = probabilities[randomNum][0];
    letters[1] = probabilities[randomNum][1];
}

void getFirstLetterByHighestProbability(int letters[2]) {
    //get highest probability
    double max = 0.0;
    for (int i = 65; i < 91; i++) { // 65 - A, 91 - Z, 122 - z
        for (int j = 65; j < 91; j++) {
            if (dataArray[i][j] > max) {
                max = dataArray[i][j];
                letters[0] = i;
                letters[1] = j;
            }
        }
    }

}

void getNextLetterByHighestProbability(int letters[2]) {
    //get highest probability
    double max = 0.0;
    int i = letters[0];
    for (int j = 65; j < 122; j++) {
        if (dataArray[i][j] > max) {
            max = dataArray[i][j];
            letters[1] = j;
        }
    }

}

void getNextLetterByPercentageProbability(int letters[2]) {
    double probabilities[256] = {0};
    int i = letters[0];
    int counter = 0;
    for (int j = 65; j < 122; j++) {
        if (dataArray[i][j] > requiredPercentage) {
            probabilities[counter++] = j;
        }
    }

    //random Buchstabe aus array zuweisen
    unsigned int randomNum = 257;
    randomNum = getRandomNum(counter);

    letters[1] = probabilities[randomNum];
}

void initializeRandomNumSeed() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    int seed = tv.tv_usec;

    srand(seed);
}

unsigned int getRandomNum(int limit) {
    return (unsigned int) rand() % limit;
}

//old
void readFile() {
    FILE *file = fopen("../ngrammeRandom.csv", "r");
    char doubleString[15] = "";
    char character;
    int c;
    int counterData = 0;

    if (file == NULL) printf("File not found"); //could not open file

    bool isProbability = false;
    bool isCharacter = true;
    int i = 0;

    while ((c = fgetc(file)) != EOF) {

        if (isProbability) {

            if (c == '\n') {
                isProbability = false;
                double probability = 0.0;
                sscanf(doubleString, "%lf", &probability);

                data[counterData].character = character;
                data[counterData++].probability = probability;
            } else {
                doubleString[i++] = (char) c;
            }

        }

        if ((char) c == ',') {
            isProbability = true;
            isCharacter = false;
            i = 0;
        }

        if ((char) c == '\n') {
            isCharacter = true;
        }

        if (isCharacter) {
            character = (char) c;
        }

    }

    printf("Finished reading");
}


// ------------------------------------------------- Erzeugen von Testdaten -------------------------------------------------

void createNgramme() {
    FILE *datei;
    int c;

    // Datei öffnen
    datei = fopen("../file.txt", "r");

    if (datei == NULL) {
        printf("Error: File not found");
    } else {
        // Zeichenweise lesen und ausgeben
        while ((c = fgetc(datei)) != EOF) {
            putchar(c);
            ngramArray[c] += 1.0;
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
        fprintf(fptr, "%c,%lf \n", i, ngramArray[i]);
    }

    // Close the file
    fclose(fptr);
}

//create random Ngram
void randomNgram() {
    FILE *fptr;

    // Open a file in writing mode
    fptr = fopen("../ngrammeRandom.csv", "w");

    for (int i = 65; i < 123; i++) {
        for (int j = 65; j < 123; j++) {
            for (int k = 65; k < 123; k++) {
                double randomNum = (double)rand() / (double)RAND_MAX * 60000;
                double value = randomNum / 100000;
                fprintf(fptr, "%c%c%c,%lf \n", i, j, k, value);
            }
        }
    }

    // Close the file
    fclose(fptr);
}
