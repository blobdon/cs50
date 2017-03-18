#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[]) {
    
    // check for correct # arguments
    if(argc != 2) {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    
    // check key for non-letters.
    // save argv[1] to uppercase string.
    char k[strlen(argv[1])];
    for(int i = 0; i < strlen(argv[1]); i++) {
        if(!isalpha(argv[1][i])) {
            printf("Usage: ./vigenere k (alpha only)\n");
            return 1;
        }
        k[i] = toupper(argv[1][i]);
    }
    
    printf("plaintext: ");
    string p = get_string();
    
    string c = p;
    int pl = strlen(p);
    int kl = strlen(k);
    int j = 0;          // index for k
    int delta;          // for zeroing the alphabet
    
    // implement vigenere's algorithm, c[i] = (p[i] + k[i]) mod 26
    for(int i = 0; i < pl; i++) {
        if(isalpha(p[i])) {
            delta = (isupper(p[i])) ? 65 : 97; // to zero alphabet (A-65, a=97)
            c[i] = ((p[i] - delta + k[j++] - 65) % 26 ) + delta;
            if(j >= kl) {j = 0;} // cycle k[j] back to beginning
        } else {
            c[i] = p[i]; // non-alphas are untouched
        }
    }
    
    printf("ciphertext: %s\n", c);
    return 0;
}