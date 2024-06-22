#ifndef PROGRAMMIER_PROJEKT_MAIN_H
#define PROGRAMMIER_PROJEKT_MAIN_H

#define TXTLENGTH 10
#define MAX_PREFIX_LEN 10
#define MAX_CHARS 256

typedef struct {
    char next_char;
    double probability;
} NgramEntry;

typedef struct {
    char prefix[MAX_PREFIX_LEN + 1];
    NgramEntry* entries;
    int entry_count;
} Ngram;

void writeInFile();
void readFile();
void createNgramme();
void randomNgram();
void readFileRandom();
void getFirstLetterByHighestProbability(char characters[2]);
void getNextLetterByHighestProbability(char characters[2]);
void generateText();
void getNextLetterByPercentageProbability(char characters[2]);
void getFirstcharactersByPercentageProbability(char characters[2]);
unsigned int getRandomNum(int counter);
void initializeRandomNumSeed();
void getFirstLetterByUser(char* characters);
//void readFileNew();
int readFileWithMalloc();
void getNextLine(FILE*);
int getNextLetterByPercentageProbabilityWithMalloc(char characters[3], Ngram* ngrams, int ngramCount, int ngramSize);
void generateTextWithMalloc();
void freeMalloc();

int getNgramCount();




int parse_ngram_model(const char* filename, Ngram* ngrams, int* ngram_count, int* ngram_size);
const int arraySize = 128;
double ngramArray[128];
int ngram = 2;
double dataArray[256][256];
char text[TXTLENGTH];
int textLength = TXTLENGTH;
double requiredPercentage = 0.0;
int textGenMode = 0; //0 = highest prob., 1 = Random value over percentage
double* dataArrayPointer[256];
char lineTest[13];
int outer_size = 256;
int middle_size = 256;
int inner_size = 256;
double*** arrayMalloc;

#endif //PROGRAMMIER_PROJEKT_MAIN_H
