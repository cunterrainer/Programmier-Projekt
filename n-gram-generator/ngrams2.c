#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 256

typedef struct {
    char pair[3]; // Pair of characters plus null terminator
    int occurrence;
    int following[N];
} Next;

// Counting occurrences of character pairs and returning the occurrence value
void countOccurence(char str[], Next pairs[]) {
    for (int i = 0; str[i] != '\0' && str[i + 1] != '\0'; i++) { // making sure there is a pair
        if (str[i]==' '||str[i + 1] == ' ') {
            continue;
        } // Skip pairs where one of the characters is a blank space

        char current_pair[3] = {str[i], str[i + 1], '\0'}; // Create a pair of characters
        char following_char=str[i+2];//the following after i+1 should be unequal to 0
        
        int found = 0;
        int pair_index=0;
        for (int j = 0; j < N; j++) {
            if (strcmp(current_pair, pairs[j].pair) == 0) {
                pairs[j].occurrence++; // Increment the occurrence of the character pair
                found = 1;
                pair_index=j;
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
        //TRTING to incremnt the occurence of the following
        if(following_char!='\0'){
            pairs[pair_index].following[following_char]++;       
        }
    }
}//broken from here


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
        for (int j=0;j<N;j++){
            
            if (pairs[i].following[j]>0){
                printf("Occurences of %c after %s= %d\n",j,pairs[i].pair,pairs[i].following[j]);
            }
        }
    }
    return 0;
}
