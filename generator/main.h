#ifndef PROGRAMMIER_PROJEKT_MAIN_H
#define PROGRAMMIER_PROJEKT_MAIN_H

#define TXTLENGTH 100

void writeInFile();
void readFile();
void createNgramme();
void randomNgram();
void readFileRandom();
void getFirstLetterByHighestProbability(int letters[2]);
void getNextLetterByHighestProbability(int letters[2]);
void generateText();
void getNextLetterByPercentageProbability(int letters[2]);
void getFirstLettersByPercentageProbability(int letters[2]);
unsigned int getRandomNum(int counter);
void initializeRandomNumSeed();
void getFirstLetterByUser(int letters[2]);
//void readFileNew();
int readFileWithMalloc();
void getNextLine(FILE*);
void getNextLetterByPercentageProbabilityWithMalloc(int letters[3]);
void generateTextWithMalloc();
void freeMalloc();
int letters[3];

typedef struct Next {
    char character;
    double probability;
} Next;

const int arraySize = 128;
double ngramArray[128];
int ngram = 2;
Next data[256];
double dataArray[256][256];
char text[TXTLENGTH];
int textLength = TXTLENGTH;
double requiredPercentage = 0.1264;
int textGenMode = 0; //0 = highest prob., 1 = Random value over percentage
double* dataArrayPointer[256];
char lineTest[13];
int outer_size = 256;
int middle_size = 256;
int inner_size = 256;
double*** arrayMalloc;

#endif //PROGRAMMIER_PROJEKT_MAIN_H
