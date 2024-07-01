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

    //get filename
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

    return 1;
}

int getNgramCount() {
    printf("Welches Ngramm einlesen [2-9]:");
    int ngram;
    scanf("%d",&ngram);
    return ngram;
}

void generateText(Ngram* ngrams, int ngramCount, int ngramSize) {
    char characters[MAXNGRAMSIZE] = "";
    ngramSize -= 1;

    printf("Ersten %d Buchstaben eingeben:", ngramSize);
    scanf("%s",&characters);

    puts("");

    //check if prefix is valid
    if (!getNextLetterByPercentageProbability(characters, ngrams, ngramCount, ngramSize)) {
        //no valid prefix - get random start prefix
        printf("No Character found for given prefix: %s\n", characters);
        getRandomPrefix(characters, ngrams, ngramCount, ngramSize);
    }

    //add prefix to text
    for (int index = 0; index < ngramSize; index++) {
        text[index] = (char) characters[index];
    }

    characters[ngramSize] = '\000';

    for (int i = ngramSize; i < TXTLENGTH; i++) {
        if (!getNextLetterByPercentageProbability(characters, ngrams, ngramCount, ngramSize)) {
            printf("No Character found for last prefix: %s\n", characters);
            break;
        }
        text[i] = characters[ngramSize];

        //fill characters with prefix and clear remains
        for (int index = 0; index < MAXNGRAMSIZE; index++) {
            characters[index] = (index < ngramSize) ? characters[index+1] : '\000';
        }

        //Stop at .
        if (text[i] == '.') {
            break;
        }
    }
    puts("");
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

    //get random char from charcaterPool
    unsigned int randomNum = 1025;
    randomNum = getRandomNum(counter);

    characters[ngramSize] = characterPool[randomNum];

    return 1;
}

void getRandomPrefix(char characters[10], Ngram* ngrams, int ngramCount, int ngramSize) {
    unsigned int num = getRandomNum(ngramCount);

    for (int i = 0; i < ngramSize; i++) {
        characters[i] = ngrams[num].prefix[i];
    }
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
