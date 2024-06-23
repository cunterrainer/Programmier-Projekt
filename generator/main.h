#ifndef PROGRAMMIER_PROJEKT_MAIN_H
#define PROGRAMMIER_PROJEKT_MAIN_H

#define TXTLENGTH 100
#define MAX_PREFIX_LEN 10
#define MAX_CHARS 256
#define MAXNGRAMSIZE 10

typedef struct {
    char next_char;
    double probability;
} NgramEntry;

typedef struct {
    char prefix[MAX_PREFIX_LEN + 1];
    NgramEntry* entries;
    int entry_count;
} Ngram;


unsigned int getRandomNum(int counter);
void initializeRandomNumSeed();
int getNextLetterByPercentageProbability(char characters[3], Ngram* ngrams, int ngramCount, int ngramSize);
void generateTextWithMalloc(Ngram* ngrams, int ngramCount, int ngramSize);
int getNgramCount();
int parse_ngram_model(const char* filename, Ngram* ngrams, int* ngram_count, int* ngram_size);
char text[TXTLENGTH];
double requiredPercentage = 0.0;

#endif //PROGRAMMIER_PROJEKT_MAIN_H
