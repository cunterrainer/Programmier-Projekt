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
void readFileNew();

#endif //PROGRAMMIER_PROJEKT_MAIN_H
