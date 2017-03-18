/**
 * Implements a dictionary's functionality.
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"
#include "trie.c"

unsigned int wcount = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    current = root;
    for (int i = 0, x = 0, len = strlen(word); i < len; i++)
    {
        // get index for trie node for this character
        x = (word[i] == '\'') ? 26: tolower(word[i]) - 97;
        // check whether that letter continues the tree, 
        // i.e, is still valid in dictionary
        if (current->letter[x] == NULL)
        {
            return false;
        }
        // move cursor to that letter node
        current = current->letter[x];
    }
    // word has finished
    return current->isword;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE *dfp = fopen(dictionary, "r");
    if (dfp == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    
    root = malloc(sizeof(node));
    current = root;
    
    // LENGTH + 2 to fit the \n and the \0 returned by fgets 
    char buf[LENGTH + 2];
    while (fgets(buf, LENGTH + 2, dfp) != NULL)
    {
        current = root;
        int i = 0;
        int x = 0;
        do 
        {
            x = (buf[i] == '\'') ? 26: tolower(buf[i]) - 97;
            if (current->letter[x] == NULL) 
            {
                current->letter[x] = malloc(sizeof(node));
            }
            current = current->letter[x];
        } 
        while(buf[++i] != '\n');
        current->isword = true;
        wcount++;
    }
    current = root;
    
    fclose(dfp);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 
 * if not yet loaded.
 */
unsigned int size(void)
{
    return wcount;
}

// rfreenode recursively frees the memory of the given node 
// and all its children nodes.
void rfreenode(node *ptr)
{
    for (int i = 0; i < ALPHA_LENGTH; i++)
    {
        if (ptr->letter[i] != NULL)
        {
            rfreenode(ptr->letter[i]);
        }
    }
    free(ptr);
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{   
    rfreenode(root);
    return true;
}
