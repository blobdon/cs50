/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

const int TOP = 65536;

/**
* Recursive binary search for value.
* Returns true if found, o/w false.
*/
bool rsearch(int value, int values[], int lower, int upper) 
{
    int middle = lower + ((upper - lower ) / 2);
    if(values[middle] == value) {
        return true;
    } else if(upper - lower < 1) {
        return false;
    } else if(values[middle] < value) {
        return rsearch(value, values, middle + 1, upper);
    } else if(values[middle] > value) {
        return rsearch(value, values, lower, middle - 1);
    }
    return false;
}

/**
 * Returns true if value is in array of n values, else false.
 * Farmed out to recursive binary search.
 */
bool search(int value, int values[], int n)
{
    if(n <= 0) {return false;}
    return rsearch(value, values, 0, n-1);
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // Save occurences of each value in counting array.
    // Cant use constant to define array length
    // b/c no mutable array sizes at compile time & const is mutable.
    int count[65536] = {0};
    for(int v = 0; v < n; v++) {
        count[values[v]]++;
    }
    for(int c = 0; c < TOP; c++) {
    }
    // add numbers back to values[] in order
    for(int c = 0, v = 0; c < TOP; c++) { // each # in counter
        for(int i = 0; i < count[c]; i++) {// each count of that #
            values[v++] = c; // save # to values[] once, move to next spot
        }
    }
    for(int i = 0; i < n; i++){
    }
    return;
}
