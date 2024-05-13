#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 256

typedef struct {
    char character;
    int occurrence;
    int following[N]; // array to store the occurrences of characters following this character
    double probability[N]; //array to store probabilities
} Next;

// Function to count occurrences of characters and return the occurrence value
void countOccurence(char str[], Next letter[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        char current_char = str[i];
        char following_char = str[i + 1];
        letter[current_char].character = current_char; // Record the character
        letter[current_char].occurrence++; // Increment the occurrence of the current character

        if (following_char != '\0') {
            letter[current_char].following[following_char]++;
        }
    }
}

// Function to count the probability of each character showing up after the recorded current character
void countProbability(Next letter[]) {
    for (int i = 0; i < N; i++) {
        if (letter[i].occurrence > 0) {
            int total_following = 0; // Total occurrences of characters following this character
            for (int j = 0; j < N; j++) {
                total_following += letter[i].following[j]; // Accumulate total occurrences
            }
            for (int j = 0; j < N; j++) {
                if (letter[i].following[j] > 0) {
                    // Calculate probability by dividing occurrences of j after i by the total following i
                    letter[i].probability[j] = (double)letter[i].following[j] / total_following;
                }
            }
        }
    }
}

int main() {
    char str[] = "Hi this a test string";
    Next letter[N] = {0}; // Initialize all occurrences to zero

    printf("String: %s\n", str);
    countOccurence(str, letter);
    countProbability(letter);

    // Printing occurrences and probabilities of each character
    for (int i = 0; i < N; i++) {
        if (letter[i].occurrence > 0) {
            printf("Occurrence of letter %c = %d\n", letter[i].character, letter[i].occurrence);
            for (int j = 0; j < N; j++) {
                if (letter[i].following[j] > 0) {
                    printf("Occurrences of %c after %c = %d\n", j, letter[i].character, letter[i].following[j]);
                    printf("Probability of %c after %c = %lf\n", j, letter[i].character, letter[i].probability[j]);
                }
            }
        }
    }
    return 0;
}


