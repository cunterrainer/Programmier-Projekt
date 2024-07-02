#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>

#define N 256


typedef struct trienode {
    struct trienode *children[N];
    int occurrence;
} trienode;


typedef struct printed_ngram {
    char prefix[N];
    char suffix[2];  // Suffix is one character
    struct printed_ngram *next;
} printed_ngram;


// Function to create a new trie node
trienode *createnode() {
    trienode *new_node = malloc(sizeof(trienode));
    for (int i = 0; i < N; i++) {
        new_node->children[i] = NULL;
    }
    new_node->occurrence = 0;
    return new_node;
}

void insert_ngram(trienode **root, int n, const char *prefix) {
    if (*root == NULL) {
    *root = createnode();//*root=new-node :means the root noe contains the address of the newnode
    }
    // Insert the prefix into the trie and update occurrences
        trienode *current = *root;
            for (int j = 0; j < n - 1; j++) {
                unsigned char index = (unsigned char)prefix[j];
                if (current->children[index] == NULL) {
                    current->children[index] = createnode();
                }
                current = current->children[index];
            }
            current->occurrence ++;
}

// Function to create a new printed n-gram node
printed_ngram *create_printed_ngram_node() {
    printed_ngram *new_printed_ngram_node = malloc(sizeof(printed_ngram));
    new_printed_ngram_node->next = NULL;
    for (int i = 0; i < N; i++) {
        new_printed_ngram_node->prefix[i] = '\0';
    }
    new_printed_ngram_node->suffix[0] = '\0';
    new_printed_ngram_node->suffix[1] = '\0';
    return new_printed_ngram_node;
}

// Function to check if an n-gram has already been printed
bool is_printed(printed_ngram *head, char *prefix, char *suffix) {
    printed_ngram *current = head;
    while (current != NULL) {
        if (strncmp(current->prefix, prefix, N) == 0 && strncmp(current->suffix, suffix, 2) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Function to mark an n-gram as printed
void insert_printed_ngram(printed_ngram **head, char *prefix, char *suffix) {
    printed_ngram *new_node = create_printed_ngram_node();
    strncpy(new_node->prefix, prefix, N);
    strncpy(new_node->suffix, suffix, 2);

    new_node->next = *head;
    *head = new_node;
}


// Calculate the total occurrences of n-grams following a given prefix
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

// Print the probabilities of n-grams following a given prefix
void print_ngram_probabilities(trienode *root, const char *prefix, int prefix_len, printed_ngram **printed_head) {
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

    printf("%s_", prefix);
    for (int i = 0; i < N; i++) {
        if (current->children[i] != NULL) {
            char suffix[2] = {i, '\0'};
            if (!is_printed(*printed_head, (char *)prefix, suffix)) {
                double probability = (double)current->children[i]->occurrence / total_following;
                printf("%c:%.4f_",i, probability);
                insert_printed_ngram(printed_head, (char *)prefix, suffix);
            }
        }
    }
    puts(" ");
}
#define TTT 6
int main() {
    trienode *root = NULL;
    char *str = "ababcdaab";

    printed_ngram *printed_head = NULL;

    // Insert n-grams of length 2 to TTT
    for (int n = 2; n <= TTT; n++) {
        for (int i = 0; i <= strlen(str) - n; i++) {

            char prefix[n];
            strncpy(prefix, str + i, n - 1);
            prefix[n - 1] = '\0';

            char suffix[2] = {str[i + n - 1], '\0'};

        insert_ngram(&root,n,prefix);

    }
}
    // Print probabilities for n-grams of length 2 to TTT
    for (int n = 2; n <= TTT; n++) {
        printf("\nProbabilities for %d-grams:\n", n);
        for (int i = 0; i <= strlen(str) - n; i++) {
            char prefix[n];
            strncpy(prefix, str + i, n - 1);
            prefix[n - 1] = '\0';

            print_ngram_probabilities(root, prefix, n - 1, &printed_head);
        }
    }

    return 0;
}
