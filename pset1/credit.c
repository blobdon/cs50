#include <cs50.h>
#include <stdio.h>

// pow10 returns 10^p
long long pow10(int p) {
    long long out = 1;
    for(int i=0; i<p; i++) {
        out = out * 10;
    }
    return out;
}

// digitn returns the nth digit of the given number
long long digitn(long long c, int n) {
    return (c % pow10(n) ) / pow10(n - 1);
}

// luhn conducts Luhn's Algorithm, returning 1 for valid numbers 0 otherwise.
int luhn(long long c) {
    int l = 0;
    for(int i=1; i<=16; i++) {
        // l = l + (i % 2) ? digitn(c, i) : digitn(2*digitn(c, i),2) + digitn(2*digitn(c,i),1);
        if(i % 2){
            l = l + digitn(c, i);
        } else {
            l = l + digitn(2*digitn(c, i),1) + digitn(2*digitn(c,i),2);
        }
    }
    // return !(l % 10);
    return (l % 10) ? 0 : 1;
}

int main(void)
{
    printf("Number: ");
    long long cc = get_long_long();
    
    if(cc >= pow10(12) && cc < pow10(13)) { // Has 13 Digits - VISA
        if(digitn(cc, 13) == 4) { // first digit = 4
            if(luhn(cc)) {
                printf("VISA\n");
                return 0;
            }
        }
    } else if (cc >= pow10(14) && cc < pow10(15)) { // Has 15 Digits - AMEX
        int first2 = 10 * digitn(cc, 15) + digitn(cc, 14);
        if(first2 == 34 || first2 == 37) { // first 2 digits = 34 or 37
            if(luhn(cc)) {
                printf("AMEX\n");
                return 0;
            }       
        }
    } else if (cc >= pow10(15) && cc < pow10(16)) { // Has 16 Digits - VISA or MC
        if(digitn(cc, 16) == 4) { // first digit = 4 - VISA
            if(luhn(cc)) {
                printf("VISA\n");
                return 0;
            }
        }
        int first2 = 10 * digitn(cc, 16) + digitn(cc, 15);
        if(first2==51 || first2==52 || first2==53 || first2 == 54 || first2 == 55) { // MC
            if(luhn(cc)) {
                printf("MASTERCARD\n");
                return 0;
            }
        }
    }
    printf("INVALID\n");
}