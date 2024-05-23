#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 256


typedef struct trienode {
    struct trienode *children[N];
    bool word_end;
    int occurrence;
} trienode;

// Function to create a new trie node
trienode *createnode() {
    trienode *new_node = (trienode *)malloc(sizeof(*new_node));
    for (int i = 0; i < N; i++) {
        new_node->children[i] = NULL; // set all to null
    }
    new_node->word_end = false;
    new_node->occurrence = 0;
    return new_node; // it returns the pointer that points to the new node
}

//Function to insert n-grams (2 to 5) into the trie
void insert_ngram(trienode **root, const char *text, int n) {
    if (*root == NULL) {
        *root = createnode();
    }

    int length = strlen(text);

    for (int i = 0; i <= length - n; i++) {
        bool valid_ngram = true;
        for (int j = 0; j < n; j++) {
            if (text[i + j] == ' ') {
                valid_ngram = false;
                break; // Skip n-grams containing spaces
            }
        }
        if (!valid_ngram) {
            continue;
        }

        trienode *current = *root;
        for (int j = 0; j < n; j++) {
            unsigned char index = (unsigned char)text[i + j];
            if (current->children[index] == NULL) {
                current->children[index] = createnode();
            }
            current = current->children[index];
        }

        current->occurrence++;
        current->word_end = true;
    }
}

//Calculate the total occurrences of n-grams following a specific prefix
int calculate_total_following(trienode *root, const char *prefix, int prefix_len) {
    if (root == NULL) {
        return 0;
    }

    trienode *current = root;
    for (int i = 0; i < prefix_len; i++) {
        unsigned char index = (unsigned char)prefix[i];
        if (current->children[index] == NULL) {
            return 0;
        }
        current = current->children[index];
    }

    int total = 0;
    for (int i = 0; i < N; i++) {
        if (current->children[i] != NULL) {
            total += current->children[i]->occurrence;
        }
    }
    return total;
}

//Print the probabilities of n-grams following a given prefix
void print_ngram_probabilities(trienode *root, const char *prefix, int prefix_len) {
    trienode *current = root;
    for (int i = 0; i < prefix_len; i++) {
        unsigned char index = (unsigned char)prefix[i];
        if (current->children[index] == NULL) {
            return;
        }
        current = current->children[index];
    }

    int total_following = calculate_total_following(root, prefix, prefix_len);
    if (total_following == 0) {
        return;
    }

    for (int i = 0; i < N; i++) {
        if (current->children[i] != NULL) {
            double probability = (double)current->children[i]->occurrence / total_following;
            printf("Probability of '%c' after '%s' is %.4f\n", i, prefix, probability);
        }
    }
}

int main() {
    trienode *root = NULL;
    char *str = "hi this is a test for a strong string";

    // Insert n-grams of length 2 to 5
    for (int n = 2; n <= 5; n++) {
        insert_ngram(&root, str, n);
    }

    // Print probabilities for n-grams of length 2 to 5
    for (int n = 2; n <= 5; n++) {
        printf("\nProbabilities for %d-grams:\n", n);
        for (int i = 0; i < strlen(str) - n + 1; i++) {
            char prefix[n];
            strncpy(prefix, str + i, n - 1);
            prefix[n - 1] = '\0';
            print_ngram_probabilities(root, prefix, n - 1);
        }
    }

    return 0;
}
