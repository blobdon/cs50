#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// poke compares the target hash to the hash of the given key
int poke(string k, string t)
{
    if (strcmp(t, crypt(k, t)) == 0)
    {
        printf("%s\n", k);
        return 1;
    }
    return 0;
}

int main(int argc, string argv[])
{
    
    // check correct # arguments
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    
    char key[5] = {'\0','\0','\0','\0'};
    char ltr[53] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int len = strlen(ltr);
    
    // try all combinations of letters in a 1,2,3,and 4 char key
    for (int i = 0; i < len; i++) 
    {
        key[0] = ltr[i];
        for (int j = 0; j < len; j++)
        {
            key[1] = ltr[j];
            for (int k = 0; k < len; k++)
            {
                key[2] = ltr[k];
                for (int l = 0; l < len; l++)
                {
                    key[3] = ltr[l];
                    if (poke(key, argv[1]) == 1) 
                    {
                        return 0;
                    }
                }
                key[3] = '\0';
                if (poke(key, argv[1]) == 1)
                {
                    return 0;
                }
            }
            key[2] = '\0';
            if (poke(key, argv[1]) == 1)
            {
                return 0;
            }
        }
        key[1] = '\0';
        if (poke(key, argv[1]) == 1) 
        {
            return 0;
        }
    }
    return 1;
}