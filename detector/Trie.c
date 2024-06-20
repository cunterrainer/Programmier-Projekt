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
    return new_node; // newnode is a pointer that pointes to the newly allocated tTrienode 
}

//Function to insert n-grams (2 to 5) into the trie
void insert_ngram(trienode **root, const char *text, int n) {//pointer to a pointer because I want to manipulate the root
    if (*root == NULL) {
        *root = createnode();//*root=new-node :means the root noe contains the address of the newnode
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

        trienode *current = *root;//current points to the same Trienode that root points to
        for (int j = 0; j < n; j++) {
            unsigned char index = (unsigned char)text[i + j];
            if (current->children[index] == NULL) {
                current->children[index] = createnode();
            }
            current = current->children[index];//updates the current pointer to point to the childnode of the new character
        }
//incremnt the occurence when the ngram is over
        current->occurrence++;
        current->word_end = true;
    }
}

//Calculate the total occurrences of n-grams following a specific prefix
int calculate_total_following(trienode *root, const char *prefix, int prefix_len) {
    if (root == NULL) {
        return 0;
    }

    trienode *current = root; //current pointer points to the 
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

    printf("%s_", prefix);
    for (int i = 0; i < N; i++) {
        if (current->children[i] != NULL) {
            double probability = (double)current->children[i]->occurrence / total_following;
            printf("%c:%.4f_", i, probability);
        }
    }
    puts("");
}

#define ttt 5
int main() {
    trienode *root = NULL;
    char *str = "The passage of time is marked by change, and with change comes the inevitability of aging and mortality. This awareness of our finite existence imbues time with a sense of urgency, prompting us to seek meaning and purpose in our lives. The philosopher Martin Heidegger described this as being - toward - death, the idea that our consciousness of mortality shapes our understanding of time and existence. In contrast, Eastern philosophies, such as Buddhism, emphasize the importance of living in the present moment, free from the constraints of past regrets and future anxieties. This mindfulness approach suggests that by embracing the present, we can transcend the linear perception of time and find a deeper sense of peace and fulfillment";

    // Insert n-grams of length 2 to 5
    for (int n = 2; n <= 5; n++) {
        insert_ngram(&root, str, n);
    }

    // Print probabilities for n-grams of length 2 to 5
    for (int n = ttt; n <= ttt; n++) {
        //printf("\nProbabilities for %d-grams:\n", n);
        for (int i = 0; i < strlen(str) - n + 1; i++) {
            char prefix[n];
            strncpy(prefix, str + i, n - 1);
            prefix[n - 1] = '\0';
            print_ngram_probabilities(root, prefix, n - 1);
        }
    }

    return 0;
}
