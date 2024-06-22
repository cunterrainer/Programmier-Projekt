#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include<sys/time.h>
#include "main.h"
#include <string.h>


int main() {
    initializeRandomNumSeed();

//    randomNgram();
//    int ngramCount = getNgramCount();
    int ngramCount = 0;
    int ngramSize = 0;
    Ngram ngrams[MAX_CHARS];

    if (!parse_ngram_model("../detector/ngrams/2_a_2.txt", ngrams, &ngramCount, &ngramSize)) {
        return 0;
    }

    generateTextWithMalloc(ngrams, &ngramCount, &ngramSize);

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

int compare_ngrams(const void* a, const void* b)
{
    Ngram* ngramA = (Ngram*)a;
    Ngram* ngramB = (Ngram*)b;
    return strcmp(ngramA->prefix, ngramB->prefix); // >0 if first non-matching char in str1 is greater than in str2 <0 if lower
}

void sort_ngrams(Ngram* ngrams, int ngram_count)
{
    qsort(ngrams, ngram_count, sizeof(Ngram), compare_ngrams);
}

int parse_ngram_model(const char* filename, Ngram* ngrams, int* ngram_count, int* ngram_size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror(filename);
        return false;
    }

    int count = 0;
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n') continue;  // Skip empty lines

        // Extract prefix
        char* prefix_end = strchr(line, '_');
        if (prefix_end == NULL) continue;  // Skip invalid lines

        const int prefix_len = prefix_end - line;
        *ngram_size = prefix_len + 1;

        strncpy(ngrams[count].prefix, line, prefix_len);
        ngrams[count].prefix[prefix_len] = '\0';

        ngrams[count].entry_count = 0;
        ngrams[count].entries = NULL;

        const char* token = strtok(prefix_end + 1, "_:\n");  // Skip the prefix and the underscore
        while (token != NULL) {
            ngrams[count].entries = realloc(ngrams[count].entries, (ngrams[count].entry_count + 1) * sizeof(NgramEntry));
            if (ngrams[count].entries == NULL) {
                fprintf(stderr, "Failed to allocate memory for ngram entries\n");
                return false;
            }

            ngrams[count].entries[ngrams[count].entry_count].next_char = token[0];
            token = strtok(NULL, "_:\n");
            ngrams[count].entries[ngrams[count].entry_count].probability = atof(token);
            token = strtok(NULL, "_:\n");
            ngrams[count].entry_count++;
        }

        count++;
    }

    *ngram_count = count;
    fclose(file);

    sort_ngrams(ngrams, *ngram_count);
    return 1;
}

int getNgramCount() {
    //Abfrage welches Ngramm eingelesen werden soll
    printf("Welches Ngramm einlesen [2-5]:");
    int ngram;
    scanf("%d",&ngram);
    return ngram;
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


void getFirstLetterByUser(char* characters){
    printf("Ersten zwei Buchstabe eingeben:");
    char firstcharacters[2];
    scanf("%s",&firstcharacters);
    characters[0] = firstcharacters[0];
    characters[1] = firstcharacters[1];
}


void generateTextWithMalloc(Ngram* ngrams, int* ngramCount, int* ngramSize) {
    char characters[3];

//    getFirstLetterByUser(characters);
    printf("Ersten zwei Buchstabe eingeben:");
    char firstcharacters[2];
    scanf("%s",&firstcharacters);
    characters[0] = firstcharacters[0];
    characters[1] = firstcharacters[1];
    //end

    text[0] = (char) characters[0];
    text[1] = (char) characters[1];

    for (int i = 2; i < TXTLENGTH; i++) {
        int blblbl = 0;
        int adf = 0; //32759
        int dff = 0; //-519602112
        int blbdlbl = 0; //32759
        int wer = 0; //-519692835
        if (!getNextLetterByPercentageProbabilityWithMalloc(characters, ngrams, *ngramCount, *ngramSize)) {
            break;
        }
        text[i] = characters[2];
        characters[0] = characters[1];
        characters[1] = characters[2];

//        if (text[i] == 'd') {
//            break;
//        }
    }

    printf("\n%s", text);
    //freeMalloc();
}

int getNextLetterByPercentageProbabilityWithMalloc(char characters[3], Ngram* ngrams, int ngramCount, int ngramSize) {
    char characterPool[1024] = {0};
    char prefix[2];
    prefix[0] = characters[0];
    prefix[1] = characters[1];
    int counter = 0;

    for (int i = 0; i < ngramCount; i++) {
        if ((int) strcmp(ngrams[i].prefix,prefix)) continue;

        for (int j = 0; j < ngrams[i].entry_count; j++) {
            if (ngrams[i].entries->probability < requiredPercentage) continue;
            characterPool[counter] = ngrams[i].entries->next_char;
            counter++;
        }
        break;
    }

    if (counter == 0) return 0;

    //random Buchstabe aus array zuweisen
    unsigned int randomNum = 1025;
    randomNum = getRandomNum(counter);

    characters[2] = characterPool[randomNum];

    return 1;
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
