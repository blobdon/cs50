#include <stdio.h>
#include <cs50.h>

void repeatchar(char c, int n) {
    for (int i = 0; i < n; i++) {
        putchar(c);
    }
}

int main(void) {
    
    int h = -1;
    while(h < 0 || h > 23) {
        printf("Height: ");
        h = get_int();
    }
    
    for(int r = 1; r <= h; r++) {
        repeatchar(' ', h -r);
        repeatchar('#', r);
        repeatchar(' ', 2);
        repeatchar('#', r);
        putchar('\n');
    }
}