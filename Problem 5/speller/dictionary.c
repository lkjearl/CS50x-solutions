// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table, A0-A9, B0-B9; where numeric is the %10 of asci
const unsigned int N = 26*10;

// Hash table
node *table[N];

// for size func
unsigned int counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    node *checker = table[hash(word)];
    if (checker != NULL)
    {
        if (strcasecmp(checker->word, word) == 0)
        {
            return true;
        }
        checker = checker->next;
    }
    return false;

    //*try for-loop*
    //for (node*checker2 = table[hash(word)]; (strcasecmp(checker, word) != true); checker2->next)
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // A0-A9, B0-B9; where numeric is the % 10 of asci sum
    // so every alphabet has 10 variants
    unsigned int sum = 0;
    unsigned int mod = 0;
    unsigned int pos = 0;

    int letter = toupper(word[0]) - 'A';

    for (int i=0; i < strlen(word); i++)
    {
        sum += toupper(word[i]);
    }
    mod = sum % 10;
    pos = (letter *10) + mod;

    return pos;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    FILE *dic = fopen(dictionary, "r");
    if (dic == NULL)
    {
        return false;
    }

    char *buffer = malloc(LENGTH+1);
    if (buffer == NULL)
    {
        return false;
    }

    while (fscanf(dic, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, buffer);
        n->next = table[hash(buffer)];
        table[hash(buffer)] = n;
        counter++;
    }
    fclose(dic);
    free(buffer);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (counter >0)
    {
        return counter;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *ptr = NULL;
    node *temp = NULL;
    for (int i = 0; i < N; i++)
    {
        ptr = table[i];
        while (ptr != NULL)
        {
            temp = ptr->next;
            free(ptr);
            ptr = temp;
        }
    }
    if (ptr == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}
