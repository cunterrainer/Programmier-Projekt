#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 256

typedef struct {
    char pair[3]; // Pair of characters plus null terminator
    int occurrence;
} Next;

// Counting occurrences of character pairs and returning the occurrence value
void countOccurence(char str[], Next pairs[]) {
    for (int i = 0; str[i] != '\0' && str[i + 1] != '\0'; i++) { // Ensure there is a pair
        if (str[i]==' '||str[i + 1] == ' ') {
            continue;
        } // Skip pairs where the second character is a space

        char current_pair[3] = {str[i], str[i + 1], '\0'}; // Create a pair of characters
        
        int found = 0;
        for (int j = 0; j < N; j++) {
            if (strcmp(current_pair, pairs[j].pair) == 0) {
                pairs[j].occurrence++; // Increment the occurrence of the character pair
                found = 1;
                break;
            }
        }
        
        // If the pair is not found, add it to pairs array
        if (!found) {
            for (int j = 0; j < N; j++) {
                if (pairs[j].occurrence == 0) {
                    strcpy(pairs[j].pair, current_pair);
                    pairs[j].occurrence = 1;
                    break;
                }
            }
        }
    }
}

int main() {
    char str[] = "Hi this a test string with strings";
    Next pairs[N] = {0}; // Initialize all occurrences to zero

    printf("String: %s\n", str);
    countOccurence(str, pairs);

    // Printing occurrences of each character pair
    for (int i = 0; i < N; i++) {
        if (pairs[i].occurrence > 0) {
            printf("Occurrences of %s = %d\n", pairs[i].pair, pairs[i].occurrence);
        }
    }
    
    return 0;
}
