#include <stdio.h>
#include <stdbool.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

//for 'size' function
int sizeCount = 0;

//Sets up pointer for linked list
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;
node *head[4096];

//hash function
int hash_func(const char *word)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash = (hash << 2) ^ word[i];
    }
    return hash % 4096;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int n = strlen(word);
    char lowercased_word[n + 1];
    strcpy(lowercased_word, word);

    //copy string, make it lowercase
    for (int i = 0; i <= n; i++)
    {
        lowercased_word[i] = tolower(lowercased_word[i]);
    }

    //checking if the word matches the result from dictionary
    int result = 0;
    int index = hash_func(lowercased_word);
    node *cursor = head[index];

    while (cursor != NULL)
    {
        result = strcmp(cursor->word, lowercased_word);
        if (result == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //opening the dictionary
    FILE *inptr = fopen(dictionary, "r");

    //check file for null
    if (inptr == NULL)
    {
        printf("There is a problem with the provided dictionary file.");
        return false;
    }

    //setting all the head pointers to NULL
    for (int i = 0; i < 4096; i++)
    {
        head[i] = NULL;
    }

    //scan each word
    char word[LENGTH + 1];
    while (fscanf(inptr, "%s", word) != EOF)
    {
        //hash the word
        int index = hash_func(word);

        //creating a node for each word
        node *new_node = malloc(sizeof(node));

        //malloc should succeed
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        //add dictionary word to the linked list
        strcpy(new_node->word, word);
        new_node->next = NULL;

        if (head[index])
        {
            new_node->next = head[index];
            head[index] = new_node;
        }

        // in case of head is pointing to null
        else
        {
            head[index] = new_node;
            new_node->next = NULL;
        }
        sizeCount++;
    }
    //free memory (word)
    fclose(inptr);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return sizeCount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < 4096; i++)
    {
        node *cursor = head[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
        head[i] = NULL;
    }
    return true;
}