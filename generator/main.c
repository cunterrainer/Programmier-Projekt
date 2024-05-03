#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include<sys/time.h>

#define TXTLENGTH 100

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


int main() {
//    createNgramme();
//    readFile();
//    randomNgram();
    readFileRandom();
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
 * - für mehr als zwei Buchstaben Funktionen anpassen
 * - testen
 * - buchstaben vorgeben
 * - zeilenumbrüche
 * - header datei
 * - baumstruktur
 * - Zeilenanfang markieren, z.b. durch festes Zeichen
 *      - generieren von Sätzen
 * - ...
 * */


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
    getFirstLettersByPercentageProbability(letters);

    text[0] = (char) letters[0];
    text[1] = (char) letters[1];
    letters[0] = letters[1];

    for (int i = 2; i < textLength; i++) {
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
            double randomNum = (double)rand() / (double)RAND_MAX * 60000;
            double value = randomNum / 100000;
            fprintf(fptr, "%c%c,%lf \n", i, j, value);
        }
    }

    // Close the file
    fclose(fptr);
}
