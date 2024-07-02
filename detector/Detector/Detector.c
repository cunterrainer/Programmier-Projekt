#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARS 256
#define MAX_PREFIX_LEN 10

typedef struct
{
    char next_char;
    double probability;
} NgramEntry;


typedef struct
{
    char prefix[MAX_PREFIX_LEN + 1];
    NgramEntry* entries;
    int entry_count;
} Ngram;


int compare_ngrams(const void* a, const void* b)
{
    Ngram* ngramA = (Ngram*)a;
    Ngram* ngramB = (Ngram*)b;
    return strcmp(ngramA->prefix, ngramB->prefix); // >0 if first non-matching char in str1 is greater than in str2 <0 if lower
}


void sort_ngrams(Ngram* ngrams, int ngram_count)
{
    qsort(ngrams, ngram_count, sizeof(Ngram), compare_ngrams);
}


bool parse_ngram_model(const char* filename, Ngram* ngrams, int* ngram_count, int * ngram_size)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        perror(filename);
        return false;
    }

    int count = 0;
    char line[1024];

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n') continue;  // Skip empty lines

        // Extract prefix
        char* prefix_end = strchr(line, '_');
        if (prefix_end == NULL) continue;  // Skip invalid lines

        const int prefix_len = prefix_end - line;
        *ngram_size = prefix_len + 1;

        strncpy(ngrams[count].prefix, line, prefix_len);
        ngrams[count].prefix[prefix_len] = '\0';

        ngrams[count].entry_count = 0;
        ngrams[count].entries = NULL;

        const char* token = strtok(prefix_end + 1, "_:\n");  // Skip the prefix and the underscore
        while (token != NULL)
        {
            ngrams[count].entries = realloc(ngrams[count].entries, (ngrams[count].entry_count + 1) * sizeof(NgramEntry));
            if (ngrams[count].entries == NULL)
            {
                fprintf(stderr, "Failed to allocate memory for ngram entries\n");
                return false;
            }
            
            ngrams[count].entries[ngrams[count].entry_count].next_char = token[0];
            token = strtok(NULL, "_:\n");
            ngrams[count].entries[ngrams[count].entry_count].probability = atof(token);
            token = strtok(NULL, "_:\n");
            ngrams[count].entry_count++;
        }

        count++;
    }

    *ngram_count = count;
    fclose(file);

    sort_ngrams(ngrams, *ngram_count);
    return true;
}


char* read_input_text(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        perror(filename);
        return false;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    if (length == -1)
    {
        perror(filename);
        return false;
    }
    fseek(file, 0, SEEK_SET);

    char* text = malloc(length + 1);
    if (text == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for input text\n");
        return false;
    }

    fread(text, 1, length, file);
    text[length] = '\0';
    fclose(file);
    return text;
}


// Quelle: https://www.geeksforgeeks.org/binary-search/
int search_ngrams(Ngram* ngrams, int ngram_count, const char* prefix)
{
    int low = 0;
    int high = ngram_count - 1;

    while (low <= high)
    {
        const int middle = (low + high) / 2;
        int cmp = strcmp(ngrams[middle].prefix, prefix);
        if (cmp == 0)
        {
            return middle;
        }
        else if (cmp < 0) // ngrams.prefix is less than prefix
        {
            low = middle + 1;
        }
        else
        {
            high = middle - 1;
        }
    }
    return -1;
}


// Quelle: https://machinelearningmastery.com/cross-entropy-for-machine-learning/
double calculate_cross_entropy(const char* text, Ngram* ngrams, int ngram_count, int ngram_size)
{
    double probability = 0.0;
    const int text_len = strlen(text);
    int ngrams_used = 0;

    for (int i = 0; i <= text_len - ngram_size; i++)
    {
        char prefix[MAX_PREFIX_LEN + 1];
        strncpy(prefix, &text[i], ngram_size - 1);
        prefix[ngram_size - 1] = '\0';
        char next_char = text[i + ngram_size - 1];

        // Find the ngram for prefix
        const int j = search_ngrams(ngrams, ngram_count, prefix);
        bool found = false;

        if (j != -1)
        {
            for (int k = 0; k < ngrams[j].entry_count; k++)
            {
                if (ngrams[j].entries[k].next_char == next_char)
                {
                    probability += log2(ngrams[j].entries[k].probability); // using log because otherwise the values would be too small to get proper results
                    found = true;
                    ngrams_used++;
                    break;
                }
            }
        }
        

        if (!found)
        {
            probability += log2(0.001);  // Assign a small probability if the n-gram is not found
            ngrams_used++;
        }
    }

    if (ngrams_used == 0)
        return INFINITY; // No n-grams found

    return -probability / ngrams_used; // Return the average negative log probability
}


double calculate_perplexity(const char* text_source, Ngram* ngrams, int ngrams_len, int ngrams_size)
{
    char* input = read_input_text(text_source);
    if (input == NULL)
    {
        return INFINITY;
    }

    const double cross_entropy = calculate_cross_entropy(input, ngrams, ngrams_len, ngrams_size); // n-gram length + 1 e.g. A_b:0.1 n=2
    const double perplexity = pow(2, cross_entropy);
    free(input);

    printf("Cross-Entropy: %.6f\n", cross_entropy);
    printf("Perplexity: %.6f\n\n", perplexity);
    return perplexity;
}


int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Usage: %s [ngram-source] [text-source]...\n", argv[0]);
        return 0;
    }

    int ngrams_len = 0;
    int ngram_size = 0;
    Ngram ngrams[MAX_CHARS];
    if (!parse_ngram_model(argv[1], ngrams, &ngrams_len, &ngram_size))
    {
        return 0;
    }

    const char* lowest_perplexity_text = NULL;
    double lowest_perplexity = INFINITY;
    for (int i = 2; i < argc; ++i)
    {
        printf("%s\n", argv[i]);
        const double perplexity = calculate_perplexity(argv[i], ngrams, ngrams_len, ngram_size);
        if (perplexity < lowest_perplexity)
        {
            lowest_perplexity = perplexity;
            lowest_perplexity_text = argv[i];
        }
    }
    printf("Most likely match: %s\n", lowest_perplexity_text);

    for (int i = 0; i < ngrams_len; ++i)
    {
        free(ngrams[i].entries);
    }
    return 0;
}