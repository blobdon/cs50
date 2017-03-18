#include <cs50.h>
#include <ctype.h> // for toupper()
#include <stdio.h>
#include <string.h> // for strlen()

int main(void) {
    
    string s = get_string();
    
    for(int i = 0, l = strlen(s); i < l; i++) {
        // is a letter
        if(s[i] != ' ') {
            // at beginning or after space
            if(i == 0 || s[i-1] == ' ') {
                putchar(toupper(s[i]));
            }
        }
    }
    
    putchar('\n');
}

