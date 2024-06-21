#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include<sys/time.h>
#include "main.h"



int main() {
    randomNgram();
    initializeRandomNumSeed();

    generateTextWithMalloc();

    return 0;
}

/*todo:
 * - buchstaben vorgeben (ein Buchstabe done)
 * - Zeilenanfang markieren, z.b. durch festes Zeichen
 *      - generieren von Sätzen
 *      - zeilenumbrüche
*  - in array pointer speichern
 *      - array pointer -> pointer -> double wert / Null (done)
 *      - für mehr als zwei Buchstaben Funktionen anpassen
 * - ...
 *
 * Done:
 * - header datei (done)
 * - testen (done)
 *
 * */

// Function to create a new node
Node *createnode() {
    Node *new_node = (Node *)malloc(sizeof(*new_node));
    for (int i = 0; i < 256; i++) {
        new_node->children[i] = NULL;
    }
    new_node->word_end = false;
    new_node->occurrence = 0;
    return new_node;
}

void readFileNode() {
    //Abfrage welches Ngramm eingelesen werden soll
    printf("Welches Ngramm einlesen [2-5]:");
    int ngram;
    scanf("%d",&ngram);
    Node ngramNode;

    //
    FILE* fptr;
    // Open a file in writing mode
    fptr = fopen("../ngrammeRandom.csv", "r");
    char line[13];
    int isEOF = 0;

    // Define the size of the outer array
    arrayMalloc = (double***)malloc(outer_size * sizeof(double**));

    int trash = 256; //todo: fgets ändert diesen wert auf 10, WARUM?!??!?! Wird er entfernt, wird die Variable darüber auf 10 gesetzt

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
        if (isEOF == 1) {
            arrayMalloc[i] = NULL;
            continue;
        }

        if (i != firstLetter) {
            arrayMalloc[i] = NULL;
            continue;
        }

        if (oldFirstLetter != firstLetter) {
            arrayMalloc[i] = (double**) malloc(middle_size * sizeof(double *));
            oldFirstLetter = firstLetter;
        }

        for (int j = 0; j < middle_size; j++) {
            if (isEOF == 1) {
                arrayMalloc[i][j] = NULL;
                continue;
            }

            if (j != secondLetter) {
                arrayMalloc[i][j] = NULL;
                continue;
            }

            if (oldSecondLetter != secondLetter) {
                arrayMalloc[i][j] = (double*) malloc(inner_size * sizeof(double));
                oldSecondLetter = secondLetter;
            }

            for (int k = 0; k < inner_size; k++) {
                if (k != thirdLetter) continue;

                arrayMalloc[i][j][k] = probability;  // Assign a simple value based on index
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

    // Close the file
    fclose(fptr);
}



int readFileWithMalloc() {
    FILE* fptr;
    // Open a file in writing mode
    fptr = fopen("../ngrammeRandom.csv", "r");
    char line[13];
    int isEOF = 0;

    // Define the size of the outer array
    arrayMalloc = (double***)malloc(outer_size * sizeof(double**));

    int trash = 256; //todo: fgets ändert diesen wert auf 10, WARUM?!??!?! Wird er entfernt, wird die Variable darüber auf 10 gesetzt

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
        if (isEOF == 1) {
            arrayMalloc[i] = NULL;
            continue;
        }

        if (i != firstLetter) {
            arrayMalloc[i] = NULL;
            continue;
        }

        if (oldFirstLetter != firstLetter) {
            arrayMalloc[i] = (double**) malloc(middle_size * sizeof(double *));
            oldFirstLetter = firstLetter;
        }

        for (int j = 0; j < middle_size; j++) {
            if (isEOF == 1) {
                arrayMalloc[i][j] = NULL;
                continue;
            }

            if (j != secondLetter) {
                arrayMalloc[i][j] = NULL;
                continue;
            }

            if (oldSecondLetter != secondLetter) {
                arrayMalloc[i][j] = (double*) malloc(inner_size * sizeof(double));
                oldSecondLetter = secondLetter;
            }

            for (int k = 0; k < inner_size; k++) {
                if (k != thirdLetter) continue;

                arrayMalloc[i][j][k] = probability;  // Assign a simple value based on index
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

    // Close the file
    fclose(fptr);

    return 0;
}

void freeMalloc() {
    // Free the allocated memory to avoid memory leaks
    for (int i = 0; i < outer_size; i++) {
        if (arrayMalloc[i] == NULL) {
            continue;
        }
        for (int j = 0; j < middle_size; j++) {
            if (arrayMalloc[i][j] == NULL) {
                continue;
            }
            free(arrayMalloc[i][j]);
        }
        free(arrayMalloc[i]);
    }
    free(arrayMalloc);
}


void getFirstLetterByUser(char characters[3]){
    printf("Ersten zwei Buchstabe eingeben:");
    char firstcharacters[2];
    scanf("%s",&firstcharacters);
    characters[0] = firstcharacters[0];
    characters[1] = firstcharacters[1];
}


void generateTextWithMalloc() {
    readFileWithMalloc();

    getFirstLetterByUser(characters);
    text[0] = (char) characters[0];
    text[1] = (char) characters[1];

    for (int i = 2; i < TXTLENGTH; i++) {
        int blblbl = 0;
        int adf = 0; //32759
        int dff = 0; //-519602112
        int blbdlbl = 0; //32759
        int wer = 0; //-519692835
        getNextLetterByPercentageProbabilityWithMalloc(characters);
        text[i] = characters[2];
        characters[0] = characters[1];
        characters[1] = characters[2];

        if (text[i] == 'd') {
            break;
        }
    }

    printf("\n%s", text);
    //freeMalloc();
}

void getNextLetterByPercentageProbabilityWithMalloc(char characters[3]) {
    double probabilities[1024] = {0};
    int first = characters[0];
    int second = characters[1];
    int counter = 0;

    for (int third = 0; third < inner_size; third++) {
        if (arrayMalloc[first][second][third] > requiredPercentage) {
            if (counter == 1024) break;
            probabilities[counter] = third;
            counter++;
        }
    }

    //random Buchstabe aus array zuweisen
    unsigned int randomNum = 1025;
    randomNum = getRandomNum(counter);

    characters[2] = probabilities[randomNum];
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

// ------------------------------------------------- Erzeugen von Testdaten -------------------------------------------------
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
