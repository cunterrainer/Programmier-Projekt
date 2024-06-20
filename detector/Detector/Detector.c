/*
A_b:0.7_c:0.1 ...
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

#define MAX_CHARS 256

typedef struct {
    char following_char;
    double probability;
} NgramEntry;

typedef struct {
    char current_char;
    NgramEntry* entries;
    int entry_count;
} Ngram;


bool parse_ngram_model(const char* filename, Ngram* ngrams, int* ngram_count)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        perror(filename);
        return false;
    }

    char line[1024];
    int count = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n') continue;  // Skip empty lines
        
        ngrams[count].current_char = line[0];
        ngrams[count].entry_count = 0;
        ngrams[count].entries = NULL;
        
        char* token = strtok(line + 2, "_:\n");  // Skip the current_char and the underscore
        while (token)
        {
            ngrams[count].entries = realloc(ngrams[count].entries, (ngrams[count].entry_count + 1) * sizeof(NgramEntry));
            ngrams[count].entries[ngrams[count].entry_count].following_char = token[0];
            token = strtok(NULL, "_: \n");
            ngrams[count].entries[ngrams[count].entry_count].probability = atof(token);
            token = strtok(NULL, "_: \n");
            ngrams[count].entry_count++;
        }

        // Normalize probabilities
        //double total_probability = 0.0;
        //for (int i = 0; i < ngrams[count].entry_count; i++)
        //{
        //    total_probability += ngrams[count].entries[i].probability;
        //}
        //for (int i = 0; i < ngrams[count].entry_count; i++)
        //{
        //    ngrams[count].entries[i].probability = ngrams[count].entries[i].probability / total_probability;
        //}

        count++;
    }

    *ngram_count = count;
    fclose(file);
    return true;
}


void printngram(Ngram* ngrams, int* ngram_count)
{
    for (int i = 0; i < *ngram_count; ++i)
    {
        printf("%c ", ngrams[i].current_char);
        for (int k = 0; k < ngrams[i].entry_count; ++k)
        {
            printf("%c %.5f ", ngrams[i].entries[k].following_char, ngrams[i].entries[k].probability);
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
    fseek(file, 0, SEEK_SET);

    *text = malloc(length + 1);
    fread(*text, 1, length, file);
    (*text)[length] = '\0';

    fclose(file);
    return true;
}


double calculate_cross_entropy(const char* text, Ngram* ngrams, int ngram_count)
{
    double log_total_probability = 0.0;
    int len = strlen(text);
    int ngrams_used = 0;

    for (int i = 0; i < len - 1; i++)
    {
        char current_char = text[i];
        char next_char = text[i + 1];

        // Find the ngram for current_char
        int found = 0;
        for (int j = 0; j < ngram_count; j++)
        {
            if (ngrams[j].current_char == current_char)
            {
                for (int k = 0; k < ngrams[j].entry_count; k++)
                {
                    if (ngrams[j].entries[k].following_char == next_char)
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
    {
        return INFINITY; // No n-grams found
    }

    return -log_total_probability / ngrams_used; // Return the average negative log probability
}


double calculate_perplexity(double cross_entropy) {
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
    int b;
    char* input = NULL;
    if (!parse_ngram_model("ngram1.txt", ngrams, &b) || !read_input_text("text2.txt", &input))
    {
        return 0;
    }
    printngram(ngrams, &b);
    printf("%s\n", input);


    double cross_entropy = calculate_cross_entropy(input, ngrams, b);
    double perplexity = calculate_perplexity(cross_entropy);

    // Use a predefined baseline perplexity (e.g., perplexity of random text)
    double baseline_perplexity = 100.0; // This is an example value, adjust as needed

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

    //printf("Probability that the text was written by the same person: %e\n", probability*100);

    for (int i = 0; i < b; ++i)
    {
        free(ngrams[i].entries);
    }
    free(input);
    getchar();
    return 0;
}