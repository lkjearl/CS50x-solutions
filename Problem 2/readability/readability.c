#include <ctype.h>
#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string input);
int count_words(string input);
int count_sentences(string input);

int main(void)
{
    //get string text
    string text = get_string("Text: ");
    //access the counters
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    //grade text
    int grade = round(0.0588 * (((float) letters / (float) words) * 100) - 0.296 * (((float) sentences / (float) words) * 100) - 15.8);
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }

}

int count_letters(string input)
{
    int letters = 0;
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (isalpha(input[i]))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string input)
{
    int words = 0;
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (isspace(input[i]))
        {
            words++;
        }
    }
    words ++;
    return words;
}

int count_sentences(string input)
{
    int sentences = 0;
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (input[i] == '.' || input[i] == '!' || input[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}