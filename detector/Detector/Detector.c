#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARS 256
#define MAX_PREFIX_LEN 10
#define MAX_LINE_LEN 1024

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

    char line[MAX_LINE_LEN];
    int count = 0;
    *ngram_size = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n') continue;  // Skip empty lines

        // Extract prefix
        char* prefix_end = strchr(line, '_');
        if (prefix_end == NULL) continue;  // Skip invalid lines
        int prefix_len = prefix_end - line;
        *ngram_size = prefix_len + 1;
        strncpy(ngrams[count].prefix, line, prefix_len);
        ngrams[count].prefix[prefix_len] = '\0';

        ngrams[count].entry_count = 0;
        ngrams[count].entries = NULL;

        char* token = strtok(prefix_end + 1, "_:\n");  // Skip the prefix and the underscore
        while (token)
        {
            ngrams[count].entries = realloc(ngrams[count].entries, (ngrams[count].entry_count + 1) * sizeof(NgramEntry));
            if (ngrams[count].entries == NULL)
            {
                fprintf(stderr, "Failed to allocate memory for ngram entries\n");
                return false;
            }
            
            ngrams[count].entries[ngrams[count].entry_count].next_char = token[0];
            token = strtok(NULL, "_: \n");
            ngrams[count].entries[ngrams[count].entry_count].probability = atof(token);
            token = strtok(NULL, "_: \n");
            ngrams[count].entry_count++;
        }

        count++;
    }

    *ngram_count = count;
    fclose(file);
    return true;
}


void printngram(Ngram* ngrams, int ngram_count)
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


bool read_input_text(const char* filename, char** text)
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

    *text = malloc(length + 1);
    if (*text == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for input text\n");
        return false;
    }

    fread(*text, 1, length, file);
    (*text)[length] = '\0';

    fclose(file);
    return true;
}



double calculate_cross_entropy(const char* text, Ngram* ngrams, int ngram_count, int ngram_size)
{
    double log_total_probability = 0.0;
    int len = strlen(text);
    int ngrams_used = 0;
    for (int i = 0; i <= len - ngram_size; i++)
    {
        char prefix[MAX_PREFIX_LEN + 1];
        strncpy(prefix, &text[i], ngram_size - 1);
        prefix[ngram_size - 1] = '\0';
        char next_char = text[i + ngram_size - 1];

        // Find the ngram for prefix
        int found = 0;
        for (int j = 0; j < ngram_count; j++)
        {
            if (strcmp(ngrams[j].prefix, prefix) == 0)
            {
                for (int k = 0; k < ngrams[j].entry_count; k++)
                {
                    if (ngrams[j].entries[k].next_char == next_char)
                    {
                        log_total_probability += log(ngrams[j].entries[k].probability);
                        found = 1;
                        ngrams_used++;
                        break;
                    }
                }
                break;
            }
        }

        if (!found)
        {
            log_total_probability += log(0.01);  // Assign a small probability if the n-gram is not found
            ngrams_used++;
        }
    }

    if (ngrams_used == 0)
        return INFINITY; // No n-grams found

    return -log_total_probability / ngrams_used; // Return the average negative log probability
}


double calculate_perplexity(double cross_entropy)
{
    return exp(cross_entropy);
}


double calculate_probability_from_perplexity(double perplexity, double baseline_perplexity)
{
    if (perplexity > baseline_perplexity)
    {
        return 0.0; // Very low probability if perplexity is higher than baseline
    }
    return 100.0 * (1.0 - (perplexity / baseline_perplexity)); // Normalize to percentage
}


int main()
{
    Ngram ngrams[MAX_CHARS];
    int ngrams_len = 0;
    char* input = NULL;
    int ngram_size;
    if (!parse_ngram_model("../ngrams/1_a_1.txt", ngrams, &ngrams_len, &ngram_size) || !read_input_text("../ngrams/1_a.txt", &input))
    {
        return 0;
    }
    printngram(ngrams, ngrams_len);
    printf("%s\n", input);


    double cross_entropy = calculate_cross_entropy(input, ngrams, ngrams_len, ngram_size); // n-gram length + 1 e.g. A_b:0.1 n=2
    double perplexity = calculate_perplexity(cross_entropy);


    double baseline_perplexity = 100;
    double probability = calculate_probability_from_perplexity(perplexity, baseline_perplexity);

    // Ensure the calculated probability is within valid range
    if (probability > 100.0)
    {
        probability = 100.0;
    }
    else if (probability < 0.0)
    {
        probability = 0.0;
    }


    printf("Cross-Entropy: %.6f\n", cross_entropy);
    printf("Perplexity: %.6f\n", perplexity);
    printf("Probability that the text was written by the same person: %.2f%%\n", probability);

    for (int i = 0; i < ngrams_len; ++i)
    {
        free(ngrams[i].entries);
    }
    free(input);
    getchar();
    return 0;
}