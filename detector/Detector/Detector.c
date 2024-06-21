#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARS 256
#define MAX_PREFIX_LEN 10
#define BASELINE_PERPLEXITY 100.0

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
    return true;
}


void print_ngram(Ngram* ngrams, int ngram_count)
{
    for (int i = 0; i < ngram_count; ++i)
    {
        printf("%s_", ngrams[i].prefix);
        for (int k = 0; k < ngrams[i].entry_count; ++k)
        {
            printf("%c:%.5f_", ngrams[i].entries[k].next_char, ngrams[i].entries[k].probability);
        }
        puts("");
    }
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
        bool found = false;
        for (int j = 0; j < ngram_count; j++)
        {
            if (strcmp(ngrams[j].prefix, prefix) == 0)
            {
                for (int k = 0; k < ngrams[j].entry_count; k++)
                {
                    if (ngrams[j].entries[k].next_char == next_char)
                    {
                        probability += log(ngrams[j].entries[k].probability); // using log because otherwise the values would be too small to get proper results
                        found = true;
                        ngrams_used++;
                        break;
                    }
                }
                break;
            }
        }

        if (!found)
        {
            probability += log(0.01);  // Assign a small probability if the n-gram is not found
            ngrams_used++;
        }
    }

    if (ngrams_used == 0)
        return INFINITY; // No n-grams found

    return -probability / ngrams_used; // Return the average negative log probability
}


double calculate_probability(double perplexity)
{
    if (perplexity > BASELINE_PERPLEXITY)
    {
        return 0.0; // Very low probability if perplexity is higher than baseline
    }

    const double probability = 100.0 * (1.0 - (perplexity / BASELINE_PERPLEXITY)); // Normalize to percentage

    if (probability > 100.0)
    {
        return 100.0;
    }
    else if (probability < 0.0)
    {
        return 0.0;
    }
    return probability;
}


void match(const char* text_source, Ngram* ngrams, int ngrams_len, int ngrams_size)
{
    char* input = read_input_text(text_source);
    if (input == NULL)
    {
        return;
    }

    const double cross_entropy = calculate_cross_entropy(input, ngrams, ngrams_len, ngrams_size); // n-gram length + 1 e.g. A_b:0.1 n=2
    const double perplexity = exp(cross_entropy);
    const double probability = calculate_probability(perplexity);
    free(input);

    //printf("Cross-Entropy: %.6f\n", cross_entropy);
    //printf("Perplexity: %.6f\n", perplexity);
    printf("Probability that the text was written by the same person: %.2f%%\n", probability);
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


    for (int i = 2; i < argc; ++i)
    {
        printf("Text: %s\n", argv[i]);
        match(argv[i], ngrams, ngrams_len, ngram_size);
    }


    for (int i = 0; i < ngrams_len; ++i)
    {
        free(ngrams[i].entries);
    }
    return 0;
}