#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 256


typedef struct trienode {
    struct trienode *children[N];//Array of Trienode poiters (the pointer give the address of each character in the trie, if there is no character the pointer is null)
    //bool word_end;
    int occurrence;
} trienode;

// Function to create a new trienode
trienode *createnode() {
    trienode *new_node = (trienode *)malloc(sizeof(*new_node));
    for (int i = 0; i < N; i++) {
        new_node->children[i] = NULL; // set all the pointers to the childnodes to null
    }
    //new_node->word_end = false;
    new_node->occurrence = 0;
    return new_node; // return the address of the new node
}

//Function to insert n-grams (2 to 5) into the trie
void insert_ngram(trienode **root, char *text, int n) {//pointer to a pointer to a Trie node because I want to manipulate the root
    if (*root == NULL) {//if the Trie is empty
        *root = createnode();//*root=new-node :means the root now contains the address of the newnode
    }

    int length = strlen(text);

    for (int i = 0; i <= length - n; i++) {//loop through the text and and stop at the last n-gram
        bool valid_ngram = true;
        for (int j = 0; j < n; j++) {//loop through the n-gram and check if it contains spaces
            if (text[i + j] == ' ') {
                valid_ngram = false;
                break; // Skip n-grams containing spaces
            }
        }
        if (!valid_ngram) {
            continue;
        }

        trienode *current = *root;//current points to the root node
        for (int j = 0; j < n; j++) {//loop through the n-gram and insert the characters into the trie
            unsigned char index = (unsigned char)text[i + j];
            if (current->children[index] == NULL) {
                current->children[index] = createnode();
            }
            current = current->children[index];//updates the current pointer to point to the childnode of the new character
        }
//incremnt the occurence when the ngram is over
        current->occurrence++;
        //current->word_end = true;
    }
}

//Calculate the total occurrences of n-grams following a specific prefix
int calculate_total_following(trienode *root, const char *prefix, int prefix_len) {
    if (root == NULL) {
        return 0;
    }

    trienode *current = root; //current pointer points to the same address as the root pointer
    for (int i = 0; i < prefix_len; i++) {//loop through the prefix
        unsigned char index = (unsigned char)prefix[i];
        if (current->children[index] == NULL) {//if the prefix is not in the trie exit the function
            return 0;
        }
        current = current->children[index];//update the current pointer to point to the childnode of the prefix
    }

    int total = 0;
    for (int i = 0; i < N; i++) {//loop through the children of the prefix
        if (current->children[i] != NULL) {//if there is a childnode after the prefix
            total += current->children[i]->occurrence;//add the occurence of the childnode to the total occurences of the letters after the prefix
        }
    }
    return total;
}

//Print the probabilities of n-grams following a given prefix
void print_ngram_probabilities(trienode *root, const char *prefix, int prefix_len) {
    trienode *current = root;//now the address that is stored in the root pointer is copied to the current pointer
    for (int i = 0; i < prefix_len; i++) {//loop through the prefix
        unsigned char index = (unsigned char)prefix[i];
        if (current->children[index] == NULL) {//if the prefix is not in the trie exit the function
            return;
        }
        current = current->children[index];//update the current pointer to point to the childnode of the prefix
    }

    int total_following = calculate_total_following(root, prefix, prefix_len);
    if (total_following == 0) {
        return;
    }

    for (int i = 0; i < N; i++) {
        if (current->children[i] != NULL) {//if there is a childnode after the prefix
            double probability = (double)current->children[i]->occurrence / total_following;
            printf("Probability of '%c' after '%s' is %.4f\n", i, prefix, probability);
        }
    }
}

int main() {
    trienode *root = NULL;//the trie is empty at the beginning
    char *str = "Deutschland (Vollform des Staatennamens seit 1949: Bundesrepublik Deutschland) ist ein Bundesstaat in Mitteleuropa.[6] Es hat 16 Bundesländer und ist als freiheitlich-demokratischer und sozialer Rechtsstaat verfasst. Die 1949 gegründete Bundesrepublik Deutschland stellt die jüngste Ausprägung des 1871 erstmals begründeten deutschen Nationalstaats dar. Im Rahmen der Wiedervereinigung Deutschlands wurde Berlin 1990 Bundeshauptstadt und 1991 zum Parlaments- und Regierungssitz bestimmt.";

    // Insert n-grams of length 2 to 5
    for (int n = 2; n <= 5; n++) {
        insert_ngram(&root, str, n);//&root is the address of the root pointer (NULL)
    }

    // Print probabilities for n-grams of length 2 to 5
    for (int n = 2; n <= 3; n++) {
        printf("\nProbabilities for %d-grams:\n", n);
        for (int i = 0; i <= strlen(str) - n; i++) {//loop through the text and and stop at the last n-gram
            char prefix[n];//Array to store the prefixes
            strncpy(prefix, str + i, n - 1);//starting from the ith character copy n-1 characters into the prefix array
            prefix[n - 1] = '\0';
            print_ngram_probabilities(root, prefix, n - 1);
        }
    }

    return 0;
}
