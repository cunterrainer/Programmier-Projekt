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

trienode *createnode() {
    trienode *new_letter = malloc(sizeof *new_letter);
    for (int i = 0; i < N; i++) {
        new_letter->children[i] = NULL;
    }
    new_letter->word_end = false;
    new_letter->occurrence = 0;
    return new_letter;
}

void insert_bigram(trienode **root, const char *text) {
    if (*root == NULL) {
        *root = createnode();
    }

    int length = strlen(text);
    for (int i = 0; i < length - 1; i++) {
        if (text[i] == ' ' || text[i + 1] == ' ') {
            continue; // Skip spaces for bigram creation
        }

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

void print_bigrams_helper(trienode *root, char *buffer, int depth) {
    if (root == NULL) {
        return;
    }

    if (root->word_end) {
        buffer[depth] = '\0';
        printf("bigram: %s, occurrence: %d\n", buffer, root->occurrence);
    }

    for (int i = 0; i < N; i++) {
        if (root->children[i] != NULL) {
            buffer[depth] = i;
            print_bigrams_helper(root->children[i], buffer, depth + 1);
        }
    }
}

void print_bigrams(trienode *root) {
    char buffer[3]; // Buffer to hold bigrams during traversal
    buffer[2] = '\0'; // Bigrams are always two characters long
    print_bigrams_helper(root, buffer, 0);
}

void calculate_total_occurrences(trienode *root, int *total) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < N; i++) {
        if (root->children[i] != NULL) {
            for (int j = 0; j < N; j++) {
                if (root->children[i]->children[j] != NULL) {
                    *total += root->children[i]->children[j]->occurrence;
                    calculate_total_occurrences(root->children[i]->children[j], total);
                }
            }
        }
    }
}

void print_bigrams_with_probabilities(trienode *root) {
    int total_occurrences = 0;
    calculate_total_occurrences(root, &total_occurrences);

    char buffer[3];
    buffer[2] = '\0';

    void print_with_probabilities_helper(trienode *node, char *buf, int depth) {
        if (node == NULL) {
            return;
        }

        if (node->word_end) {
            buf[depth] = '\0';
            double probability = (double)node->occurrence / total_occurrences;
            printf("bigram: %s, occurrence: %d, probability: %.4f\n", buf, node->occurrence, probability);
        }

        for (int i = 0; i < N; i++) {
            if (node->children[i] != NULL) {
                buf[depth] = i;
                print_with_probabilities_helper(node->children[i], buf, depth + 1);
            }
        }
    }

    print_with_probabilities_helper(root, buffer, 0);
}

int main() {
    trienode *root = NULL;
    char *str = "hi this is a test string";

    insert_bigram(&root, str);

    printf("Bigrams and their occurrences:\n");
    print_bigrams(root);

    printf("\nBigrams with probabilities:\n");
    print_bigrams_with_probabilities(root);

    // Freeing memory (optional but good practice)
    // Free function not implemented here for brevity

    return 0;
}
