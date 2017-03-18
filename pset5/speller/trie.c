
#include <stdbool.h>

// size of alphabet for dictionary
#define ALPHA_LENGTH 27

// node is a node in the trie dictionary
typedef struct node 
{
  bool isword;
  struct node *letter[ALPHA_LENGTH];
} 
node;

// create root node
node *root;

// create current node to maintain current location;
node *current;

/**
 * This is a lookup table I originally used to map chars to
 * the correct values for my trie nodes. I switched to the
 * arithmetic approach and there was no noticeable difference
 * so I just left it. I'm leaving this in for posterity's sake.
 * (also it took some time to get it all nice.)
 */
// int trieval[] = {
//  // gap (0-38)
//   99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  
//   99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  
//   99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  
//   99,  99,  99,  99,  99,  99,  99,  99,  99, 
//  // apostrophe (39)
//  26,  
//  // gap (40-64)
//   99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  
//   99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  
//   99,  99,  99,  99,  99,
//  // capital letters (65-90)
//   0,  1,  2,  3,  4,  5, 6,  7,  8,  9, 
//  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
//  20, 21, 22, 23, 24, 25,
//  // gap (91-96)
//   99,  99,  99,  99,  99,  99,
//  // lowercase letters (97-122)
//   0,  1,  2,  3, 4,  5,  6,  7,  8,  9, 
//  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
//  20, 21, 22, 23, 24, 25,
// };

