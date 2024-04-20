#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 256

typedef struct {
    char character;
    int occurrence;
    int following[N]; // array to store the occurrences of characters following this character
    double probability;
} Next;

// Function to count occurrences of characters and return the occurrence value
void countProbability(char str[], Next letter[]) {
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

int main() {
    char str[] = "abbbcdfgk";

    Next letter[N] = {0}; // Initialize all occurrences to zero

    printf("String: %s\n", str);

    countProbability(str, letter);

    // Printing occurrences of each character
    for (int i = 0; i < N; i++) {
        if (letter[i].occurrence > 0) {
            printf("Occurrence of letter %c = %d\n", letter[i].character, letter[i].occurrence);

            // Printing the occurrences of characters following this character
            for (int j = 0; j < N; j++) {
                if (letter[i].following[j] > 0) {
                    printf("Occurrences of %c after %c = %d\n", letter[j].character, letter[i].character, letter[i].following[j]);
                }
            }
        }
    }

    return 0;
}