#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 256

// Trie node structure definition
typedef struct trienode {
    struct trienode *children[N];
    bool word_end;
    int occurrence;
} trienode;

// Function to create a new trie node
trienode *createnode() {
    trienode *new_letter = (trienode*)malloc(sizeof *new_letter);
    for (int i = 0; i < N; i++) {
        new_letter->children[i] = NULL;
    }
    new_letter->word_end = false;
    new_letter->occurrence = 0;
    return new_letter;
}

// Function to insert bigrams into the trie
void insert_bigram(trienode **root, const char *text) {
    if (*root == NULL) {
        *root = createnode();
    }

    int length = strlen(text);
    for (int i = 0; i < length - 1; i++) {
        if (text[i] == ' ' || text[i + 1] == ' ') {
            continue; // Skip spaces for bigram creation
        }

        // Casting to avoid negative index
        unsigned char index1 = (unsigned char)text[i];
        unsigned char index2 = (unsigned char)text[i + 1];

        if ((*root)->children[index1] == NULL) {
            (*root)->children[index1] = createnode();
        }

        if ((*root)->children[index1]->children[index2] == NULL) {
            (*root)->children[index1]->children[index2] = createnode();
        }

        (*root)->children[index1]->children[index2]->occurrence++;
        (*root)->children[index1]->children[index2]->word_end = true;
    }
}

// Helper function to calculate the total occurrences of bigrams starting with a specific character
int calculate_total_following(trienode *root, unsigned char index) {
    if (root == NULL || root->children[index] == NULL) {
        return 0;
    }

    int total = 0;
    for (int i = 0; i < N; i++) {
        if (root->children[index]->children[i] != NULL) {
            total += root->children[index]->children[i]->occurrence;
        }
    }
    return total;
}

// Function to print the probabilities of each character following another character
void print_following_probabilities(trienode *root) {
    for (int i = 0; i < N; i++) {
        if (root->children[i] != NULL) {
            int total_following = calculate_total_following(root, i);
            if (total_following == 0) {
                continue;
            }

            for (int j = 0; j < N; j++) {
                if (root->children[i]->children[j] != NULL) {
                    double probability = (double)root->children[i]->children[j]->occurrence / total_following;
                    printf("Probability of '%c' after '%c' is %.4f\n", j, i, probability);
                }
            }
        }
    }
}

int main() {
    trienode *root = NULL;
    char *str = "hi this is a test string";

    insert_bigram(&root, str);

    printf("Character following probabilities:\n");
    print_following_probabilities(root);

    return 0;
}
