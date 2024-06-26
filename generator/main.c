#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include<sys/time.h>
#include "main.h"
#include <string.h>


int main() {
    initializeRandomNumSeed();

    int ngramSize = 0;
    Ngram ngrams[MAX_CHARS];

    int ngramCount = getNgramCount();

    char filename[30] = "../generator/ngrams/ngram_";
    char buffer[2];
    sprintf(buffer, "%d", ngramCount);
    strcat(filename, buffer);
    strcat(filename, ".txt");

    if (!parse_ngram_model(filename, ngrams, &ngramCount, &ngramSize)) {
        return 0;
    }

    generateText(ngrams, ngramCount, ngramSize);

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

//int compare_ngrams(const void* a, const void* b)
//{
//    Ngram* ngramA = (Ngram*)a;
//    Ngram* ngramB = (Ngram*)b;
//    return strcmp(ngramA->prefix, ngramB->prefix); // >0 if first non-matching char in str1 is greater than in str2 <0 if lower
//}
//
//void sort_ngrams(Ngram* ngrams, int ngram_count)
//{
//    qsort(ngrams, ngram_count, sizeof(Ngram), compare_ngrams);
//}

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

//    sort_ngrams(ngrams, *ngram_count);
    return 1;
}

int getNgramCount() {
    //Abfrage welches Ngramm eingelesen werden soll
    printf("Welches Ngramm einlesen [2-5]:");
    int ngram;
    scanf("%d",&ngram);
    return ngram;
}


void getFirstLetterByUser(char* characters){
    printf("Ersten zwei Buchstabe eingeben:");
    char firstcharacters[2];
    scanf("%s",&firstcharacters);
    characters[0] = firstcharacters[0];
    characters[1] = firstcharacters[1];
}


void generateText(Ngram* ngrams, int ngramCount, int ngramSize) {
    char characters[MAXNGRAMSIZE];
    ngramSize -= 1;
//    getFirstLetterByUser(characters);
    printf("Ersten %d Buchstaben eingeben:", ngramSize);
    scanf("%s",&characters);
    //end
    for (int index = 0; index < ngramSize; index++) {
        text[index] = (char) characters[index];
    }

    for (int i = ngramSize; i < TXTLENGTH; i++) {
        if (!getNextLetterByPercentageProbability(characters, ngrams, ngramCount, ngramSize)) {
            printf("\nNo Character found for last given prefix\n");
            break;
        }
        text[i] = characters[ngramSize];

        //fill characters with prefix and clear remains
        for (int index = 0; index < MAXNGRAMSIZE; index++) {
            characters[index] = (index < ngramSize) ? characters[index+1] : '\000';
        }

        //Stop at sentence end
        if (text[i] == '.') {
            break;
        }
    }

    printf("Generated Text:\n%s", text);
}

int getNextLetterByPercentageProbability(char characters[10], Ngram* ngrams, int ngramCount, int ngramSize) {
    char characterPool[1024] = {0};
    int counter = 0;

    for (int i = 0; i < ngramCount; i++) {
        if ((int) strcmp(ngrams[i].prefix,characters)) continue;

        for (int j = 0; j < ngrams[i].entry_count; j++) {
            if (ngrams[i].entries->probability < requiredPercentage) continue;
            characterPool[counter] = ngrams[i].entries[j].next_char;
            counter++;
        }
        break;
    }

    if (counter == 0) return 0;

    //random Buchstabe aus array zuweisen
    unsigned int randomNum = 1025;
    randomNum = getRandomNum(counter);

    characters[ngramSize] = characterPool[randomNum];

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
